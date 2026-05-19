// font_ansi_shadow.h
#ifndef FONT_ANSI_SHADOW_H
#define FONT_ANSI_SHADOW_H

#ifdef __cplusplus
extern "C" {
#endif

#define FONT_HEIGHT     6
#define FONT_TAB_SIZE   4
#define FONT_GAP_COUNT  0

typedef enum {
    GRADIENT_HORIZONTAL,
    GRADIENT_RAINBOW_SMOOTH,
    GRADIENT_ICE,
    GRADIENT_SPRING
} GradientMode;

void font_ansi_shadow_print(const char *text);
void font_ansi_shadow_print_gradient(const char *text, GradientMode mode);

#ifdef __cplusplus
}
#endif

#endif
