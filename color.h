#ifndef COLOR_H
#define COLOR_H

// ===== Color Enumeration =====
typedef enum {
    COLOR_BLACK = 0,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE,
    COLOR_BRIGHT_BLACK,
    COLOR_BRIGHT_RED,
    COLOR_BRIGHT_GREEN,
    COLOR_BRIGHT_YELLOW,
    COLOR_BRIGHT_BLUE,
    COLOR_BRIGHT_MAGENTA,
    COLOR_BRIGHT_CYAN,
    COLOR_BRIGHT_WHITE
} ColorName;

// ===== Color Functions =====
void set_fg_color(ColorName color);
void set_bg_color(ColorName color);
void set_fg_256(int index);
void set_bg_256(int index);
void set_fg_rgb(int r, int g, int b);
void set_bg_rgb(int r, int g, int b);
void reset_color(void);

// ===== Text Style Enable Functions =====
void set_bold(void);
void set_italic(void);
void set_underline(void);
void set_reverse(void);
void set_blink(void);

// ===== Text Style Reset Functions =====
void reset_bold(void);
void reset_italic(void);
void reset_underline(void);
void reset_reverse(void);
void reset_blink(void);

// ===== Cursor Color (OSC Sequence) =====
void set_cursor_color_rgb(int r, int g, int b);
void reset_cursor_color(void);

#endif // COLOR_H
