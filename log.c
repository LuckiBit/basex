#include "log.h"
#include <stdlib.h>
#include <string.h>

#if LOG_ENABLE_FILE
#include <errno.h>
#include <sys/stat.h>
#endif

#if defined(_WIN32)
  #include <windows.h>
  #include <io.h>
  #include <direct.h>
  #define ISATTY _isatty(_fileno(stdout))
  #define PATH_SEP '\\'
#else
  #include <unistd.h>
  #include <dirent.h>
  #include <sys/types.h>
  #define ISATTY isatty(fileno(stdout))
  #define PATH_SEP '/'
#endif

// ==========================
// Globals (initialized from macros, can be changed via setters)
// ==========================
static int            g_inited            = 0;
static FILE          *g_file              = NULL;
static log_level_t    g_min_level         = LOG_MIN_LEVEL;
static unsigned       g_prefix_console    = LOG_PREFIX_STDOUT;
static unsigned       g_prefix_file       = LOG_PREFIX_FILEOUT;
static log_rotate_mode_t g_rotate_mode    = LOG_ROTATE_MODE;
static unsigned long  g_max_size          = LOG_MAX_FILE_SIZE;
static unsigned       g_max_days          = LOG_MAX_DAYS;
static int            g_enable_colors     = LOG_ENABLE_COLORS;

// Console color map per level
static log_color_t g_level_color[5] = {
    LOG_COLOR_GREEN,          // DEBUG
    LOG_COLOR_BRIGHT_WHITE,   // INFO
    LOG_COLOR_BRIGHT_YELLOW,  // WARN
    LOG_COLOR_BRIGHT_RED,     // ERROR
    LOG_COLOR_BRIGHT_RED      // FATAL
};

// ANSI color codes
static const char *g_color_code[] = {
    "\x1b[30m","\x1b[31m","\x1b[32m","\x1b[33m","\x1b[34m","\x1b[35m","\x1b[36m","\x1b[37m",
    "\x1b[90m","\x1b[91m","\x1b[92m","\x1b[93m","\x1b[94m","\x1b[95m","\x1b[96m","\x1b[97m",
    "\x1b[39m"
};

static char g_fixed_path[512] = LOG_OUTPUT_FILE; // used in NONE/SIZE
static char g_daily_path[512] = {0};             // current daily file path

static const char *g_level_name[] = {"DEBUG","INFO","WARN","ERROR","FATAL"};

// ==========================
// Utilities
// ==========================
static int is_console_color_supported(void) {
#if defined(_WIN32)
    // Enable VT on Windows 10+
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h == INVALID_HANDLE_VALUE) return 0;
    DWORD mode = 0;
    if (!GetConsoleMode(h, &mode)) return 0;
    mode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    if (!SetConsoleMode(h, mode)) return 0;
    return 1;
#else
    return ISATTY;
#endif
}

// millis timestamp: "YYYY-MM-DD HH:MM:SS.mmm"
static void now_timestamp(char *buf, size_t len) {
#if defined(_WIN32)
    SYSTEMTIME st;
    GetLocalTime(&st);
    _snprintf(buf, (int)len, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
              (int)st.wYear, (int)st.wMonth, (int)st.wDay,
              (int)st.wHour, (int)st.wMinute, (int)st.wSecond, (int)st.wMilliseconds);
#else
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm tm;
    localtime_r(&ts.tv_sec, &tm);
    snprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d.%03ld",
             tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec, ts.tv_nsec/1000000);
#endif
}

static void today_datestr(char *ymd, size_t len) {
    time_t t = time(NULL);
    struct tm tm;
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    strftime(ymd, len, "%Y%m%d", &tm);
}

static void join_path(char *dst, size_t len, const char *dir, const char *name) {
    size_t dlen = strlen(dir);
    if (dlen && (dir[dlen-1] == '/' || dir[dlen-1] == '\\')) {
        snprintf(dst, len, "%s%s", dir, name);
    } else {
        snprintf(dst, len, "%s%c%s", dir, PATH_SEP, name);
    }
}

// recursive mkdir on both / and \ for Windows
static void mkdirs(const char *path) {
    if (!path || !*path) return;
    char tmp[512];
    size_t n = strlen(path);
    if (n >= sizeof(tmp)) n = sizeof(tmp)-1;
    memcpy(tmp, path, n);
    tmp[n] = '\0';
    for (size_t i=1; i<n; ++i) {
        if (tmp[i] == '/' || tmp[i] == '\\') {
            char c = tmp[i];
            tmp[i] = '\0';
#if defined(_WIN32)
            _mkdir(tmp);
#else
            mkdir(tmp, 0755);
#endif
            tmp[i] = c;
        }
    }
#if defined(_WIN32)
    _mkdir(tmp);
#else
    mkdir(tmp, 0755);
#endif
}

// parse "BASENAME_YYYYMMDD.log" -> returns (Y,M,D) if ok, else 0
static int parse_date_from_name(const char *fname, int *Y, int *M, int *D) {
    // find last '_' then read 8 digits
    const char *us = strrchr(fname, '_');
    const char *dot = strrchr(fname, '.');
    if (!us || !dot || dot <= us || (dot-us) != 9) return 0; // '_' + 8 digits
    int y,m,d;
    if (sscanf(us+1, "%4d%2d%2d", &y, &m, &d) != 3) return 0;
    *Y = y; *M = m; *D = d;
    return 1;
}

static void build_daily_filepath(char *out, size_t len) {
    char ymd[16];
    today_datestr(ymd, sizeof(ymd));
    char base[128];
    snprintf(base, sizeof(base), "%s_%s.log", LOG_DAILY_BASENAME, ymd);
    join_path(out, len, LOG_OUTPUT_DIR, base);
}

static long file_size_fp(FILE *fp) {
    if (!fp) return 0;
    long cur = ftell(fp);
    fseek(fp, 0, SEEK_END);
    long end = ftell(fp);
    fseek(fp, cur, SEEK_SET);
    return end;
}

// Remove files older than LOG_MAX_DAYS in LOG_OUTPUT_DIR matching "BASENAME_YYYYMMDD.log"
static void cleanup_old_daily(void) {
    if (g_max_days == 0) return;
    time_t now = time(NULL);

#if defined(_WIN32)
    char pattern[512];
    join_path(pattern, sizeof(pattern), LOG_OUTPUT_DIR, "*.log");

    WIN32_FIND_DATAW ffdw;
    WIN32_FIND_DATAA ffda;
    HANDLE h = FindFirstFileA(pattern, &ffda);
    if (h == INVALID_HANDLE_VALUE) return;
    do {
        const char *name = ffda.cFileName;
        // must start with BASENAME_
        size_t blen = strlen(LOG_DAILY_BASENAME);
        if (strncmp(name, LOG_DAILY_BASENAME, blen) != 0) continue;
        if (name[blen] != '_') continue;

        int Y,M,D;
        if (!parse_date_from_name(name, &Y, &M, &D)) continue;

        struct tm tm = {0};
        tm.tm_year = Y - 1900; tm.tm_mon = M - 1; tm.tm_mday = D;
        time_t t = mktime(&tm);
        double days = difftime(now, t) / (60*60*24);
        if (days > (double)g_max_days) {
            char full[512];
            join_path(full, sizeof(full), LOG_OUTPUT_DIR, name);
            remove(full);
        }
    } while (FindNextFileA(h, &ffda));
    FindClose(h);
#else
    DIR *d = opendir(LOG_OUTPUT_DIR);
    if (!d) return;
    struct dirent *ent;
    while ((ent = readdir(d)) != NULL) {
        if (ent->d_name[0] == '.') continue;
        size_t blen = strlen(LOG_DAILY_BASENAME);
        if (strncmp(ent->d_name, LOG_DAILY_BASENAME, blen) != 0) continue;
        if (ent->d_name[blen] != '_') continue;
        int Y,M,D;
        if (!parse_date_from_name(ent->d_name, &Y, &M, &D)) continue;

        struct tm tm = {0};
        tm.tm_year = Y - 1900; tm.tm_mon = M - 1; tm.tm_mday = D;
        time_t t = mktime(&tm);
        double days = difftime(now, t) / (60*60*24);
        if (days > (double)g_max_days) {
            char full[512];
            join_path(full, sizeof(full), LOG_OUTPUT_DIR, ent->d_name);
            remove(full);
        }
    }
    closedir(d);
#endif
}

static void open_log_file_initial(void) {
#if LOG_ENABLE_FILE
    mkdirs(LOG_OUTPUT_DIR);
    if (g_rotate_mode == LOG_ROTATE_DAILY) {
        build_daily_filepath(g_daily_path, sizeof(g_daily_path));
        g_file = fopen(g_daily_path, "a");
        cleanup_old_daily();
    } else {
        // NONE or SIZE
        // Ensure directory for LOG_OUTPUT_FILE exists
        // (mkdirs already called on LOG_OUTPUT_DIR; but FILE may contain subdirs)
        // Try to create missing subdirs from path
        char dirbuf[512];
        strncpy(dirbuf, LOG_OUTPUT_FILE, sizeof(dirbuf)-1);
        dirbuf[sizeof(dirbuf)-1] = '\0';
        char *p = dirbuf + strlen(dirbuf);
        while (p > dirbuf && *p != '/' && *p != '\\') --p;
        if (p > dirbuf) {
            *p = '\0';
            mkdirs(dirbuf);
        }
        strncpy(g_fixed_path, LOG_OUTPUT_FILE, sizeof(g_fixed_path)-1);
        g_fixed_path[sizeof(g_fixed_path)-1] = '\0';
        g_file = fopen(g_fixed_path, "a");
    }
#endif
}

static void rotate_if_needed(void) {
    if (!g_file) return;

    if (g_rotate_mode == LOG_ROTATE_SIZE) {
        long sz = file_size_fp(g_file);
        if (sz >= 0 && (unsigned long)sz >= g_max_size) {
            // truncate current file
            fclose(g_file);
            g_file = fopen(g_fixed_path, "w"); // truncate to zero
        }
    } else if (g_rotate_mode == LOG_ROTATE_DAILY) {
        char today[512];
        build_daily_filepath(today, sizeof(today));
        if (strcmp(today, g_daily_path) != 0) {
            if (g_file) fclose(g_file);
            strcpy(g_daily_path, today);
            g_file = fopen(g_daily_path, "a");
            cleanup_old_daily();
        }
    }
}

static void ensure_init(void) {
    if (g_inited) return;
    g_inited = 1;

    if (g_enable_colors && ISATTY) {
        (void)is_console_color_supported();
    }
    open_log_file_initial();

    atexit(log_close);
}


// ==========================
// Public API
// ==========================
void log_init(void) { ensure_init(); }

void log_close(void) {
    if (g_file) fclose(g_file);
    g_file = NULL;
    g_inited = 0;
}

void log_set_level(log_level_t level)         { g_min_level = level; }
void log_set_color(log_level_t level, log_color_t color) {
    if (level >= LOG_DEBUG && level <= LOG_FATAL) g_level_color[level] = color;
}
void log_set_prefix(unsigned mask_for_console) { g_prefix_console = mask_for_console; }
void log_set_file_prefix(unsigned mask_for_file) { g_prefix_file = mask_for_file; }

// Only for NONE/SIZE; DAILY ignores custom path (it uses LOG_OUTPUT_DIR+DATE)
void log_set_file(const char *filepath) {
#if LOG_ENABLE_FILE
    if (g_rotate_mode == LOG_ROTATE_DAILY) return; // ignored
    if (!filepath || !*filepath) return;
    if (g_file) { fclose(g_file); g_file = NULL; }
    strncpy(g_fixed_path, filepath, sizeof(g_fixed_path)-1);
    g_fixed_path[sizeof(g_fixed_path)-1] = '\0';
    // create dir
    char dirbuf[512];
    strncpy(dirbuf, g_fixed_path, sizeof(dirbuf)-1);
    dirbuf[sizeof(dirbuf)-1] = '\0';
    char *p = dirbuf + strlen(dirbuf);
    while (p > dirbuf && *p != '/' && *p != '\\') --p;
    if (p > dirbuf) { *p = '\0'; mkdirs(dirbuf); }
    g_file = fopen(g_fixed_path, "a");
#endif
}

// ==========================
// Format prefix
// ==========================
static int build_prefix(char *dst, size_t cap, unsigned mask,
                        log_level_t level, const char *file, int line, const char *func) {
    size_t off = 0;
    if (mask & LOG_PREFIX_TIME) {
        char ts[64]; now_timestamp(ts, sizeof(ts));
        off += (size_t)snprintf(dst+off, cap-off, "[%s] ", ts);
        if (off >= cap) return (int)off;
    }
    if (mask & LOG_PREFIX_LEVEL) {
        off += (size_t)snprintf(dst+off, cap-off, "[%s] ", g_level_name[level]);
        if (off >= cap) return (int)off;
    }
    if (mask & LOG_PREFIX_FILE) {
        off += (size_t)snprintf(dst+off, cap-off, "%s:", file);
        if (off >= cap) return (int)off;
    }
    if (mask & LOG_PREFIX_LINE) {
        off += (size_t)snprintf(dst+off, cap-off, "%d ", line);
        if (off >= cap) return (int)off;
    }
    if (mask & LOG_PREFIX_FUNC) {
        off += (size_t)snprintf(dst+off, cap-off, "(%s) ", func);
    }
    return (int)off;
}

// ==========================
// Write: both console + file
// ==========================
void log_write(log_level_t level, const char *file, int line, const char *func,
               const char *fmt, ...) {
    if (level < g_min_level) return;
    ensure_init();

    char prefix[256]; prefix[0] = '\0';
    int plen_console = build_prefix(prefix, sizeof(prefix), g_prefix_console, level, file, line, func);

    char msg[1024];
    va_list ap; va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);

#if LOG_ENABLE_CONSOLE
    // Console
    if (g_enable_colors && ISATTY) {
        fprintf(stdout, "%s%s%s\n", g_color_code[g_level_color[level]], prefix, msg);
        fputs("\x1b[0m", stdout);
    } else {
        fprintf(stdout, "%s%s\n", prefix, msg);
    }
    fflush(stdout);
#endif

#if LOG_ENABLE_FILE
    // File
    if (g_file) {
        rotate_if_needed();
        char fpref[256]; fpref[0] = '\0';
        int plen_file = build_prefix(fpref, sizeof(fpref), g_prefix_file, level, file, line, func);
        (void)plen_console; (void)plen_file;
        fprintf(g_file, "%s%s\n", fpref, msg);
        fflush(g_file);
    }
#endif
}

// ==========================
// Console only
// ==========================
void log_write_console(log_level_t level, const char *file, int line, const char *func,
                       const char *fmt, ...) {
#if LOG_ENABLE_CONSOLE
    if (level < g_min_level) return;
    ensure_init();

    char prefix[256]; prefix[0] = '\0';
    build_prefix(prefix, sizeof(prefix), g_prefix_console, level, file, line, func);

    char msg[1024];
    va_list ap; va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);

    if (g_enable_colors && ISATTY) {
        fprintf(stdout, "%s%s%s\n", g_color_code[g_level_color[level]], prefix, msg);
        fputs("\x1b[0m", stdout);
    } else {
        fprintf(stdout, "%s%s\n", prefix, msg);
    }
    fflush(stdout);
#endif
}

// ==========================
// File only
// ==========================
void log_write_file(log_level_t level, const char *file, int line, const char *func,
                    const char *fmt, ...) {
#if LOG_ENABLE_FILE
    if (level < g_min_level) return;
    ensure_init();
    if (!g_file) return;

    rotate_if_needed();

    char prefix[256]; prefix[0] = '\0';
    build_prefix(prefix, sizeof(prefix), g_prefix_file, level, file, line, func);

    char msg[1024];
    va_list ap; va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);

    fprintf(g_file, "%s%s\n", prefix, msg);
    fflush(g_file);
#endif
}
