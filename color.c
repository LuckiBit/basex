#include "color.h"
#include <stdio.h>

// ===== Default safe values =====
#define DEFAULT_COLOR COLOR_BLACK
#define DEFAULT_256_INDEX 0
#define DEFAULT_RGB_R 0
#define DEFAULT_RGB_G 0
#define DEFAULT_RGB_B 0

// ===== Color Control with safety fallback =====
void set_fg_color(ColorName color) {
    if (color < COLOR_BLACK || color > COLOR_BRIGHT_WHITE) {
        color = DEFAULT_COLOR;
    }
    if (color <= COLOR_WHITE)
        printf("\033[%dm", 30 + color);
    else
        printf("\033[%dm", 90 + (color - 8));
}

void set_bg_color(ColorName color) {
    if (color < COLOR_BLACK || color > COLOR_BRIGHT_WHITE) {
        color = DEFAULT_COLOR;
    }
    if (color <= COLOR_WHITE)
        printf("\033[%dm", 40 + color);
    else
        printf("\033[%dm", 100 + (color - 8));
}

void set_fg_256(int index) {
    if (index < 0 || index > 255) {
        index = DEFAULT_256_INDEX;
    }
    printf("\033[38;5;%dm", index);
}

void set_bg_256(int index) {
    if (index < 0 || index > 255) {
        index = DEFAULT_256_INDEX;
    }
    printf("\033[48;5;%dm", index);
}

void set_fg_rgb(int r, int g, int b) {
    if (r < 0 || r > 255) r = DEFAULT_RGB_R;
    if (g < 0 || g > 255) g = DEFAULT_RGB_G;
    if (b < 0 || b > 255) b = DEFAULT_RGB_B;
    printf("\033[38;2;%d;%d;%dm", r, g, b);
}

void set_bg_rgb(int r, int g, int b) {
    if (r < 0 || r > 255) r = DEFAULT_RGB_R;
    if (g < 0 || g > 255) g = DEFAULT_RGB_G;
    if (b < 0 || b > 255) b = DEFAULT_RGB_B;
    printf("\033[48;2;%d;%d;%dm", r, g, b);
}

void reset_color(void) {
    printf("\033[0m");
}

// ===== Text Style Enable =====
void set_bold(void) { printf("\033[1m"); }
void set_italic(void) { printf("\033[3m"); }
void set_underline(void) { printf("\033[4m"); }
void set_reverse(void) { printf("\033[7m"); }
void set_blink(void) { printf("\033[5m"); }

// ===== Text Style Reset =====
void reset_bold(void) { printf("\033[22m"); }
void reset_italic(void) { printf("\033[23m"); }
void reset_underline(void) { printf("\033[24m"); }
void reset_reverse(void) { printf("\033[27m"); }
void reset_blink(void) { printf("\033[25m"); }

// ===== Cursor Color (OSC Sequence) with safety check =====
void set_cursor_color_rgb(int r, int g, int b) {
    if (r < 0 || r > 255) r = DEFAULT_RGB_R;
    if (g < 0 || g > 255) g = DEFAULT_RGB_G;
    if (b < 0 || b > 255) b = DEFAULT_RGB_B;
    printf("\033]12;#%02X%02X%02X\007", r, g, b);
}

void reset_cursor_color(void) {
    printf("\033]112\007");
}
