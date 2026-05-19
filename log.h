#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

// ==========================
// Enable/disable outputs
// ==========================
#ifndef LOG_ENABLE_CONSOLE
#define LOG_ENABLE_CONSOLE 1   // 1 = enable console logging, 0 = disable
#endif

#ifndef LOG_ENABLE_FILE
#define LOG_ENABLE_FILE    1   // 1 = enable file logging, 0 = disable
#endif

// ==========================
// Log levels
// ==========================
typedef enum {
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
} log_level_t;

// ==========================
// Prefix bits
// ==========================
#define LOG_PREFIX_TIME   (1u << 0)
#define LOG_PREFIX_LEVEL  (1u << 1)
#define LOG_PREFIX_FILE   (1u << 2)
#define LOG_PREFIX_LINE   (1u << 3)
#define LOG_PREFIX_FUNC   (1u << 4)
#define LOG_PREFIX_ALL    (LOG_PREFIX_TIME|LOG_PREFIX_LEVEL|LOG_PREFIX_FILE|LOG_PREFIX_LINE|LOG_PREFIX_FUNC)

// ==========================
// Colors (independent names)
// ==========================
typedef enum {
    LOG_COLOR_BLACK = 0,
    LOG_COLOR_RED,
    LOG_COLOR_GREEN,
    LOG_COLOR_YELLOW,
    LOG_COLOR_BLUE,
    LOG_COLOR_MAGENTA,
    LOG_COLOR_CYAN,
    LOG_COLOR_WHITE,
    LOG_COLOR_BRIGHT_BLACK,
    LOG_COLOR_BRIGHT_RED,
    LOG_COLOR_BRIGHT_GREEN,
    LOG_COLOR_BRIGHT_YELLOW,
    LOG_COLOR_BRIGHT_BLUE,
    LOG_COLOR_BRIGHT_MAGENTA,
    LOG_COLOR_BRIGHT_CYAN,
    LOG_COLOR_BRIGHT_WHITE,
    LOG_COLOR_DEFAULT
} log_color_t;

// ==========================
// Rotation modes
// ==========================
typedef enum {
    LOG_ROTATE_NONE = 0,   // Single file, never split
    LOG_ROTATE_SIZE,       // Single file, truncate when size >= limit
    LOG_ROTATE_DAILY       // app_YYYYMMDD.log, delete files older than N days
} log_rotate_mode_t;

// ==========================
// Global configuration (edit here)
// ==========================

// Where to put logs
#ifndef LOG_OUTPUT_DIR
#define LOG_OUTPUT_DIR          "logs"
#endif

// Fixed file name for NONE/SIZE modes
#ifndef LOG_OUTPUT_FILE
#define LOG_OUTPUT_FILE         "logs/basex.log"
#endif

// Base name for DAILY mode -> file is "<dir>/<base>_YYYYMMDD.log"
#ifndef LOG_DAILY_BASENAME
#define LOG_DAILY_BASENAME      "basex"
#endif

// Rotation: NONE / SIZE / DAILY
#ifndef LOG_ROTATE_MODE
#define LOG_ROTATE_MODE         LOG_ROTATE_SIZE
#endif

// Size threshold for SIZE mode (bytes). When reached, current file is truncated.
#ifndef LOG_MAX_FILE_SIZE
#define LOG_MAX_FILE_SIZE       (5u * 1000u * 1u)
#endif

// Retention in days for DAILY mode. Files strictly older than this are removed.
#ifndef LOG_MAX_DAYS
#define LOG_MAX_DAYS            7
#endif

// Minimum level to output
#ifndef LOG_MIN_LEVEL
#define LOG_MIN_LEVEL           LOG_DEBUG
#endif

// Prefix for console/file independently
#ifndef LOG_PREFIX_STDOUT
#define LOG_PREFIX_STDOUT       LOG_PREFIX_ALL
#endif

#ifndef LOG_PREFIX_FILEOUT
#define LOG_PREFIX_FILEOUT      LOG_PREFIX_TIME
#endif

// Enable ANSI colors for console (auto-detected on Win; set 0 to hard-disable)
#ifndef LOG_ENABLE_COLORS
#define LOG_ENABLE_COLORS       1
#endif

// ==========================
// API
// ==========================

// Initialize logger (creates directories, opens file, cleans up old files if needed)
// Safe to call multiple times; also lazy-initializes on first use if omitted.
void log_init(void);

// Close file handle (optional; process exit will flush/close as well)
void log_close(void);

// Change runtime options (optional; defaults from macros above)
void log_set_level(log_level_t level);
void log_set_color(log_level_t level, log_color_t color);
void log_set_prefix(unsigned mask_for_console);
void log_set_file_prefix(unsigned mask_for_file);
void log_set_file(const char *filepath); // Only used in NONE/SIZE modes; ignored by DAILY

// Core write that sends to both console and file (if file enabled/open)
void log_write(log_level_t level, const char *file, int line, const char *func,
               const char *fmt, ...);

// Console-only
void log_write_console(log_level_t level, const char *file, int line, const char *func,
                       const char *fmt, ...);

// File-only
void log_write_file(log_level_t level, const char *file, int line, const char *func,
                    const char *fmt, ...);

// ==========================
// Convenience macros
// ==========================
#define log_debug(fmt, ...) log_write(LOG_DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...)  log_write(LOG_INFO,  __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...)  log_write(LOG_WARN,  __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) log_write(LOG_ERROR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define log_fatal(fmt, ...) log_write(LOG_FATAL, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

// Console-only
#define log_debug_console(fmt, ...) log_write_console(LOG_DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define log_info_console(fmt, ...)  log_write_console(LOG_INFO,  __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define log_warn_console(fmt, ...)  log_write_console(LOG_WARN,  __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define log_error_console(fmt, ...) log_write_console(LOG_ERROR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define log_fatal_console(fmt, ...) log_write_console(LOG_FATAL, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

// File-only
#define log_debug_file(fmt, ...) log_write_file(LOG_DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define log_info_file(fmt, ...)  log_write_file(LOG_INFO,  __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define log_warn_file(fmt, ...)  log_write_file(LOG_WARN,  __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define log_error_file(fmt, ...) log_write_file(LOG_ERROR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define log_fatal_file(fmt, ...) log_write_file(LOG_FATAL, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif
#endif // LOG_H
