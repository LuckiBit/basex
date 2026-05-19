#include "font_ansi_shadow.h"
#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/* Glyph definitions (each character: 6 lines) */
static const char *char_A[FONT_HEIGHT] = {
    " █████╗ ",
    "██╔══██╗",
    "███████║",
    "██╔══██║",
    "██║  ██║",
    "╚═╝  ╚═╝"
};

static const char *char_B[FONT_HEIGHT] = {
    "██████╗ ",
    "██╔══██╗",
    "██████╔╝",
    "██╔══██╗",
    "██████╔╝",
    "╚═════╝ "
};

/* Add C-Z letters (fill with correct glyphs, using ANSI Shadow style) */
static const char *char_C[FONT_HEIGHT] = {
    " ██████╗",
    "██╔════╝",
    "██║     ",
    "██║     ",
    "╚██████╗",
    " ╚═════╝"
};

static const char *char_D[FONT_HEIGHT] = {
    "██████╗ ",
    "██╔══██╗",
    "██║  ██║",
    "██║  ██║",
    "██████╔╝",
    "╚═════╝ "
};

static const char *char_E[FONT_HEIGHT] = {
    "███████╗",
    "██╔════╝",
    "█████╗  ",
    "██╔══╝  ",
    "███████╗",
    "╚══════╝"
};

static const char *char_F[FONT_HEIGHT] = {
    "███████╗",
    "██╔════╝",
    "█████╗  ",
    "██╔══╝  ",
    "██║     ",
    "╚═╝     "
};

static const char *char_G[FONT_HEIGHT] = {
    " ██████╗ ",
    "██╔════╝ ",
    "██║  ███╗",
    "██║   ██║",
    "╚██████╔╝",
    " ╚═════╝ "
};

static const char *char_H[FONT_HEIGHT] = {
    "██╗  ██╗",
    "██║  ██║",
    "███████║",
    "██╔══██║",
    "██║  ██║",
    "╚═╝  ╚═╝"
};

static const char *char_I[FONT_HEIGHT] = {
    "██╗",
    "██║",
    "██║",
    "██║",
    "██║",
    "╚═╝"
};

static const char *char_J[FONT_HEIGHT] = {
    "     ██╗",
    "     ██║",
    "     ██║",
    "██   ██║",
    "╚█████╔╝",
    " ╚════╝ "
};

static const char *char_K[FONT_HEIGHT] = {
    "██╗  ██╗",
    "██║ ██╔╝",
    "█████╔╝ ",
    "██╔═██╗ ",
    "██║  ██╗",
    "╚═╝  ╚═╝"
};

static const char *char_L[FONT_HEIGHT] = {
    "██╗     ",
    "██║     ",
    "██║     ",
    "██║     ",
    "███████╗",
    "╚══════╝"
};

static const char *char_M[FONT_HEIGHT] = {
    "███╗   ███╗",
    "████╗ ████║",
    "██╔████╔██║",
    "██║╚██╔╝██║",
    "██║ ╚═╝ ██║",
    "╚═╝     ╚═╝"
};

static const char *char_N[FONT_HEIGHT] = {
    "███╗   ██╗",
    "████╗  ██║",
    "██╔██╗ ██║",
    "██║╚██╗██║",
    "██║ ╚████║",
    "╚═╝  ╚═══╝"
};

static const char *char_O[FONT_HEIGHT] = {
    " ██████╗ ",
    "██╔═══██╗",
    "██║   ██║",
    "██║   ██║",
    "╚██████╔╝",
    " ╚═════╝ "
};

static const char *char_P[FONT_HEIGHT] = {
    "██████╗ ",
    "██╔══██╗",
    "██████╔╝",
    "██╔═══╝ ",
    "██║     ",
    "╚═╝     "
};

static const char *char_Q[FONT_HEIGHT] = {
    " ██████╗ ",
    "██╔═══██╗",
    "██║   ██║",
    "██║▄▄ ██║",
    "╚██████╔╝",
    " ╚══▀▀═╝ "
};

static const char *char_R[FONT_HEIGHT] = {
    "██████╗ ",
    "██╔══██╗",
    "██████╔╝",
    "██╔══██╗",
    "██║  ██║",
    "╚═╝  ╚═╝"
};

static const char *char_S[FONT_HEIGHT] = {
    " ██████╗",
    "██╔════╝",
    "╚█████╗ ",
    " ╚═══██╗",
    "██████╔╝",
    "╚═════╝ "
};

static const char *char_T[FONT_HEIGHT] = {
    "████████╗",
    "╚══██╔══╝",
    "   ██║   ",
    "   ██║   ",
    "   ██║   ",
    "   ╚═╝   "
};

static const char *char_U[FONT_HEIGHT] = {
    "██╗   ██╗",
    "██║   ██║",
    "██║   ██║",
    "██║   ██║",
    "╚██████╔╝",
    " ╚═════╝ "
};

static const char *char_V[FONT_HEIGHT] = {
    "██╗   ██╗",
    "██║   ██║",
    "██║   ██║",
    "╚██╗ ██╔╝",
    " ╚████╔╝ ",
    "  ╚═══╝  "
};

static const char *char_W[FONT_HEIGHT] = {
    "██╗    ██╗",
    "██║    ██║",
    "██║ █╗ ██║",
    "██║███╗██║",
    "╚███╔███╔╝",
    " ╚══╝╚══╝ "
};

static const char *char_X[FONT_HEIGHT] = {
    "██╗  ██╗",
    "╚██╗██╔╝",
    " ╚███╔╝ ",
    " ██╔██╗ ",
    "██╔╝╚██╗",
    "╚═╝  ╚═╝"
};

static const char *char_Y[FONT_HEIGHT] = {
    "██╗   ██╗",
    "╚██╗ ██╔╝",
    " ╚████╔╝ ",
    "  ╚██╔╝  ",
    "   ██║   ",
    "   ╚═╝   "
};

static const char *char_Z[FONT_HEIGHT] = {
    "███████╗",
    "╚══███╔╝",
    "  ███╔╝ ",
    " ███╔╝  ",
    "███████╗",
    "╚══════╝"
};

/* Digits 0-9 (ANSI Shadow style, height 6) */
static const char *char_0[FONT_HEIGHT] = {
    " ██████╗ ",
    "██╔═████╗",
    "██║██╔██║",
    "████╔╝██║",
    "╚██████╔╝",
    " ╚═════╝ "
};

static const char *char_1[FONT_HEIGHT] = {
    " ██╗",
    "███║",
    "╚██║",
    " ██║",
    " ██║",
    " ╚═╝"
};

static const char *char_2[FONT_HEIGHT] = {
    "██████╗ ",
    "╚════██╗",
    " █████╔╝",
    "██╔═══╝ ",
    "███████╗",
    "╚══════╝"
};

static const char *char_3[FONT_HEIGHT] = {
    "██████╗ ",
    "╚════██╗",
    " █████╔╝",
    " ╚═══██╗",
    "██████╔╝",
    "╚═════╝ "
};

static const char *char_4[FONT_HEIGHT] = {
    "██╗  ██╗",
    "██║  ██║",
    "███████║",
    "╚════██║",
    "     ██║",
    "     ╚═╝"
};

static const char *char_5[FONT_HEIGHT] = {
    "███████╗",
    "██╔════╝",
    "███████╗",
    "╚════██║",
    "███████║",
    "╚══════╝"
};

static const char *char_6[FONT_HEIGHT] = {
    " ██████╗ ",
    "██╔════╝ ",
    "███████╗ ",
    "██╔═══██╗",
    "╚██████╔╝",
    " ╚═════╝ "
};

static const char *char_7[FONT_HEIGHT] = {
    "███████╗",
    "╚═══██╔╝",
    "   ██╔╝ ",
    "  ██╔╝  ",
    "  ██║   ",
    "  ╚═╝   "
};

static const char *char_8[FONT_HEIGHT] = {
    " █████╗ ",
    "██╔══██╗",
    "╚█████╔╝",
    "██╔══██╗",
    "╚█████╔╝",
    " ╚════╝ "
};

static const char *char_9[FONT_HEIGHT] = {
    " █████╗ ",
    "██╔══██╗",
    "╚██████║",
    " ╚═══██║",
    " █████╔╝",
    " ╚════╝ "
};

/* Common symbols */
static const char *char_PLUS[FONT_HEIGHT] = {
    "   ",
    " █ ",
    "███",
    " █ ",
    "   ",
    "   "
};

static const char *char_MINUS[FONT_HEIGHT] = {
    "      ",
    "      ",
    "█████╗",
    "╚════╝",
    "      ",
    "      "
};

static const char *char_ASTERISK[FONT_HEIGHT] = {
    "      ",
    "▄ ██╗▄",
    " ████╗",
    "▀╚██╔▀",
    "  ╚═╝ ",
    "      "
};

/* Slash (/) */
static const char *char_SLASH[FONT_HEIGHT] = {
    "    ██╗",
    "   ██╔╝",
    "  ██╔╝ ",
    " ██╔╝  ",
    "██╔╝   ",
    "╚═╝    "
};

/* Equal (=) */
static const char *char_EQUAL[FONT_HEIGHT] = {
    "        ",
    "███████╗",
    "╚══════╝",
    "███████╗",
    "╚══════╝",
    "        "
};

/* Comma (,) */
static const char *char_COMMA[FONT_HEIGHT] = {
    "   ",
    "   ",
    "   ",
    "   ",
    "▄█╗",
    "╚═╝"
};

/* Period (.) */
static const char *char_PERIOD[FONT_HEIGHT] = {
    "   ",
    "   ",
    "   ",
    "   ",
    "██╗",
    "╚═╝"
};

/* Question mark (?) */
static const char *char_QUESTION[FONT_HEIGHT] = {
    "██████╗ ",
    "╚════██╗",
    "  ▄███╔╝",
    "  ▀▀══╝ ",
    "  ██╗   ",
    "  ╚═╝   "
};

/* Colon (:) */
static const char *char_COLON[FONT_HEIGHT] = {
    "   ",
    "██╗",
    "╚═╝",
    "██╗",
    "╚═╝",
    "   "
};

/* Exclamation (!) */
static const char *char_EXCLAMATION[FONT_HEIGHT] = {
    "██╗",
    "██║",
    "██║",
    "╚═╝",
    "██╗",
    "╚═╝"
};

/* At (@) */
static const char *char_AT[FONT_HEIGHT] = {
    " ██████╗ ",
    "██╔═══██╗",
    "██║██╗██║",
    "██║██║██║",
    "╚█║████╔╝",
    " ╚╝╚═══╝ "
};

/* Hash (#) */
static const char *char_HASH[FONT_HEIGHT] = {
    " ██╗ ██╗ ",
    "████████╗",
    "╚██╔═██╔╝",
    "████████╗",
    "╚██╔═██╔╝",
    " ╚═╝ ╚═╝ "
};

/* Dollar ($) */
static const char *char_DOLLAR[FONT_HEIGHT] = {
    "▄▄███▄▄·",
    "██╔════╝",
    "███████╗",
    "╚════██║",
    "███████║",
    "╚═▀▀▀══╝"
};

/* Percent (%) */
static const char *char_PERCENT[FONT_HEIGHT] = {
    "██╗ ██╗",
    "╚═╝██╔╝",
    "  ██╔╝ ",
    " ██╔╝  ",
    "██╔╝██╗",
    "╚═╝ ╚═╝"
};

/* Caret (^) */
static const char *char_CARET[FONT_HEIGHT] = {
    " ███╗ ",
    "██╔██╗",
    "╚═╝╚═╝",
    "      ",
    "      ",
    "      "
};

/* Ampersand (&) */
static const char *char_AMPERSAND[FONT_HEIGHT] = {
    "   ██╗   ",
    "   ██║   ",
    "████████╗",
    "██╔═██╔═╝",
    "██████║  ",
    "╚═════╝  "
};

/* Left Parenthesis ( ( ) */
static const char *char_LPAREN[FONT_HEIGHT] = {
    " ██╗",
    "██╔╝",
    "██║ ",
    "██║ ",
    "╚██╗",
    " ╚═╝"
};

/* Right Parenthesis ( ) ) */
static const char *char_RPAREN[FONT_HEIGHT] = {
    "██╗ ",
    "╚██╗",
    " ██║",
    " ██║",
    "██╔╝",
    "╚═╝ "
};

/* Left curly brace ({) */
static const char *char_LCURLY[FONT_HEIGHT] = {
    "  ██╗",
    " ██╔╝",
    "██╔╝ ",
    "╚██╗ ",
    " ╚██╗",
    "  ╚═╝"
};

/* Right curly brace (}) */
static const char *char_RCURLY[FONT_HEIGHT] = {
    "██╗  ",
    "╚██╗ ",
    " ╚██╗",
    " ██╔╝",
    "██╔╝ ",
    "╚═╝  "
};

/* Left Square Bracket [ */
static const char *char_LBRACKET[FONT_HEIGHT] = {
    "███╗",
    "██╔╝",
    "██║ ",
    "██║ ",
    "███╗",
    "╚══╝"
};

/* Right Square Bracket ] */
static const char *char_RBRACKET[FONT_HEIGHT] = {
    "███╗",
    "╚██║",
    " ██║",
    " ██║",
    "███║",
    "╚══╝"
};

/* Less Than Sign < */
static const char *char_LT[FONT_HEIGHT] = {
    "  ██╗",
    " ██╔╝",
    "██╔╝ ",
    "╚██╗ ",
    " ╚██╗",
    "  ╚═╝"
};

/* Greater Than Sign > */
static const char *char_GT[FONT_HEIGHT] = {
    "██╗  ",
    "╚██╗ ",
    " ╚██╗",
    " ██╔╝",
    "██╔╝ ",
    "╚═╝  "
};

/* Backslash (\\) */
static const char *char_BACKSLASH[FONT_HEIGHT] = {
    "██╗    ",
    "╚██╗   ",
    " ╚██╗  ",
    "  ╚██╗ ",
    "   ╚██╗",
    "    ╚═╝"
};

/* Vertical bar (|) */
static const char *char_PIPE[FONT_HEIGHT] = {
    "██╗",
    "██║",
    "██║",
    "██║",
    "██║",
    "╚═╝"
};

/* Double quote (\") */
static const char *char_QUOTE_DOUBLE[FONT_HEIGHT] = {
    "██╗ ██╗",
    "██║ ██║",
    "╚═╝ ╚═╝",
    "       ",
    "       ",
    "       "
};

/* Underscore _ */
static const char *char_UNDERSCORE[FONT_HEIGHT] = {
    "        ",
    "        ",
    "        ",
    "        ",
    "███████╗",
    "╚══════╝"
};

/* Space ( ) */
static const char *char_SPACE[FONT_HEIGHT] = {
    "    ",
    "    ",
    "    ",
    "    ",
    "    ",
    "    "
};

/* Special gap glyph (for controlling spacing between chars) */
static const char *char_GAP[FONT_HEIGHT] = {
    " ",
    " ",
    " ",
    " ",
    " ",
    " "
};

/* Return the glyph for a given char (case-insensitive). */
static const char **get_glyph(char c) {
    c = (char)toupper((unsigned char)c);
    switch (c) {
        /* Letters */
        case 'A': return char_A;
        case 'B': return char_B;
        case 'C': return char_C;
        case 'D': return char_D;
        case 'E': return char_E;
        case 'F': return char_F;
        case 'G': return char_G;
        case 'H': return char_H;
        case 'I': return char_I;
        case 'J': return char_J;
        case 'K': return char_K;
        case 'L': return char_L;
        case 'M': return char_M;
        case 'N': return char_N;
        case 'O': return char_O;
        case 'P': return char_P;
        case 'Q': return char_Q;
        case 'R': return char_R;
        case 'S': return char_S;
        case 'T': return char_T;
        case 'U': return char_U;
        case 'V': return char_V;
        case 'W': return char_W;
        case 'X': return char_X;
        case 'Y': return char_Y;
        case 'Z': return char_Z;

        /* Digits */
        case '0': return char_0;
        case '1': return char_1;
        case '2': return char_2;
        case '3': return char_3;
        case '4': return char_4;
        case '5': return char_5;
        case '6': return char_6;
        case '7': return char_7;
        case '8': return char_8;
        case '9': return char_9;

        /* Symbols */
        case '+': return char_PLUS;
        case '-': return char_MINUS;
        case '*': return char_ASTERISK;        
        case '/': return char_SLASH;
        case '=': return char_EQUAL;

        case ',': return char_COMMA;
        case '.': return char_PERIOD;
        case '?': return char_QUESTION;
        case ':': return char_COLON;
        case '!': return char_EXCLAMATION;
        case '@': return char_AT;
        case '#': return char_HASH;
        case '$': return char_DOLLAR;
        case '%': return char_PERCENT;
        case '^': return char_CARET;
        case '&': return char_AMPERSAND;
        case '(': return char_LPAREN;
        case ')': return char_RPAREN;
        case '{': return char_LCURLY;
        case '}': return char_RCURLY;
        case '[': return char_LBRACKET;
        case ']': return char_RBRACKET;
        case '<': return char_LT;
        case '>': return char_GT;
        case '\\': return char_BACKSLASH;
        case '|': return char_PIPE;
        case '"': return char_QUOTE_DOUBLE;
        case '_': return char_UNDERSCORE;
        
        case ' ': return char_SPACE;
        default:  return char_SPACE;
    }
}

static inline double clamp01(double x){ return x < 0 ? 0 : (x > 1 ? 1 : x); }
static inline int lerp_u8(int a, int b, double t){ return (int)(a + (b - a) * t + 0.5); }

/* HSV -> RGB，h:0~360, s/v:0~1 */
static void hsv_to_rgb(double h, double s, double v, int *r, int *g, int *b){
    double c = v * s;
    double x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    double m = v - c;
    double rr=0, gg=0, bb=0;
    int hi = (int)floor(h / 60.0) % 6;
    switch (hi) {
        case 0: rr=c; gg=x; bb=0; break;
        case 1: rr=x; gg=c; bb=0; break;
        case 2: rr=0; gg=c; bb=x; break;
        case 3: rr=0; gg=x; bb=c; break;
        case 4: rr=x; gg=0; bb=c; break;
        default: rr=c; gg=0; bb=x; break;
    }
    *r = (int)((rr + m) * 255 + 0.5);
    *g = (int)((gg + m) * 255 + 0.5);
    *b = (int)((bb + m) * 255 + 0.5);
}

/* 多段调色板插值（修正版） */
typedef struct { int r,g,b; double at; } Stop;
static void palette_sample(double t, const Stop *stops, int n, int *r, int *g, int *b){
    if (!stops || n <= 0){ *r=255; *g=255; *b=255; return; }

    t = clamp01(t);

    if (t <= stops[0].at){ *r=stops[0].r; *g=stops[0].g; *b=stops[0].b; return; }
    if (t >= stops[n-1].at){ *r=stops[n-1].r; *g=stops[n-1].g; *b=stops[n-1].b; return; }

    for (int i=0; i<n-1; ++i){
        if (t >= stops[i].at && t <= stops[i+1].at){
            double denom = (stops[i+1].at - stops[i].at);
            double u = (denom != 0.0) ? (t - stops[i].at) / denom : 0.0;
            u = clamp01(u);
            *r = lerp_u8(stops[i].r, stops[i+1].r, u);
            *g = lerp_u8(stops[i].g, stops[i+1].g, u);
            *b = lerp_u8(stops[i].b, stops[i+1].b, u);
            return;
        }
    }
    *r=255; *g=255; *b=255; // fallback
}

static void compute_gradient_color(int row, int col, int max_row, int max_col,
                                   GradientMode mode, int *r, int *g, int *b)
{
    double W = (max_col > 1) ? (double)(max_col - 1) : 1.0;
    double H = (max_row > 1) ? (double)(max_row - 1) : 1.0;
    double x = (double)col / W;
    double y = (double)row / H;

    switch (mode) {
        case GRADIENT_HORIZONTAL: {
            /* Horizontal gradient: 7-color evenly spaced (Purple → Blue → Cyan → Green → Yellow → Orange → Red) */
            static const Stop PAL_HORIZ[] = {
                {128,   0, 128, 0.00}, // Purple
                {  0,   0, 255, 0.16}, // Blue
                {  0, 255, 255, 0.33}, // Cyan
                {  0, 255,   0, 0.50}, // Green
                {255, 255,   0, 0.66}, // Yellow
                {255, 128,   0, 0.83}, // Orange
                {255,   0,   0, 1.00}  // Red
            };
            palette_sample(x, PAL_HORIZ, sizeof(PAL_HORIZ)/sizeof(PAL_HORIZ[0]), r,g,b);
            return;
        }

        case GRADIENT_RAINBOW_SMOOTH: {
            /* Smooth rainbow: three-phase sine wave, increased frequency to show variation at small widths */
            double a = x * 6.0 * M_PI;   // originally 2π, now 6π (3 cycles)
            *r = (int)((sin(a + 0.0) * 0.5 + 0.5) * 255);
            *g = (int)((sin(a + 2.094395102) * 0.5 + 0.5) * 255);
            *b = (int)((sin(a + 4.188790205) * 0.5 + 0.5) * 255);
            return;
        }

        case GRADIENT_ICE: {
            // Vertical position ratio
            double t = pow(y, 1.5); // non-linear gradient: top deep blue → bottom white

            // Base ice-snow gradient color
            double r_base = 0.0*(1-t) + 230*t;   // R: deep cyan → white
            double g_base = 140*(1-t) + 250*t;   // G: cyan → white
            double b_base = 160*(1-t) + 255*t;   // B: light cyan → white

            // Snowflake probability increases from top to bottom
            double snow_prob = 0.01 + 0.05 * y;
            int is_snow = ((double)rand() / RAND_MAX) < snow_prob;

            if (is_snow) {
                // Random snowflake width, 1~3 columns
                int snow_width = (rand() % 3) + 1;
                *r = *g = *b = 255;
                return;
            }

            // Ice crystal shimmer: small brightness fluctuation
            double shimmer = 0.05 * sin(5.0*x + 10.0*y); // x/y position affects brightness
            r_base = clamp01(r_base/255.0 + shimmer) * 255;
            g_base = clamp01(g_base/255.0 + shimmer) * 255;
            b_base = clamp01(b_base/255.0 + shimmer) * 255;

            // Add light cyan/gray decorations for natural feel
            double decor_prob = 0.02; // decoration probability
            if ((double)rand()/RAND_MAX < decor_prob) {
                r_base = r_base * 0.8 + 200*0.2;
                g_base = g_base * 0.8 + 220*0.2;
                b_base = b_base * 0.8 + 230*0.2;
            }

            *r = (int)r_base;
            *g = (int)g_base;
            *b = (int)b_base;
            return;
        }

        case GRADIENT_SPRING: {
            double t = y; // 0..1 vertical position
            double r_base, g_base, b_base;
        
            // --- Sky: top 70% ---
            if (t < 0.7) {
                double tt = t / 0.7; // 0..1 within sky
                r_base = 135 + 120 * tt;  // 135->255
                g_base = 206 + 30 * tt;   // 206->236
                b_base = 235 + 20 * tt;   // 235->255
            
                // --- Clouds: only in top part of sky ---
                // Clouds appear only in the top 30% of the sky  
                if (tt < 0.3) {
                    double cloud_prob = 0.05 + 0.05 * (0.3 - tt); // probability higher near top
                    int cloud_width = 4 + rand()%6;
                    int cloud_height = 1 + rand()%3;
                    if (((double)rand()/RAND_MAX) < cloud_prob){
                        if ((col % cloud_width) < cloud_width && (row % cloud_height) < cloud_height){
                            r_base = g_base = b_base = 255;
                        }
                    }
                }
            } else {
                // --- Grass: bottom 30% ---
                double tt = (t - 0.7) / 0.3; // 0..1 within grass
                r_base = 50 + 30 * tt + ((rand()%20)-10);   // 50->80 +/-10
                g_base = 180 + 50 * tt + ((rand()%20)-10);  // 180->230 +/-10
                b_base = 50 + 20 * tt + ((rand()%20)-10);   // 50->70 +/-10
            
                // --- Flowers: more frequent towards bottom ---
                double flower_prob = 0.08 + 0.25 * tt; // 8%~33%
                if (((double)rand() / RAND_MAX) < flower_prob) {
                    int color_type = rand() % 4;
                    switch(color_type){
                        case 0: r_base=255; g_base=0;   b_base=0;   break; // red
                        case 1: r_base=255; g_base=215; b_base=0;   break; // yellow
                        case 2: r_base=255; g_base=182; b_base=193; break; // pink
                        case 3: r_base=255; g_base=140; b_base=0;   break; // orange
                    }
                }
            }
        
            // --- Small shimmer effect ---
            double shimmer = 0.02 * sin(10.0*x + 10.0*y);
            r_base = clamp01(r_base/255.0 + shimmer) * 255;
            g_base = clamp01(g_base/255.0 + shimmer) * 255;
            b_base = clamp01(b_base/255.0 + shimmer) * 255;
        
            *r = (int)r_base;
            *g = (int)g_base;
            *b = (int)b_base;
            return;
        }

        default:
            *r=255; *g=255; *b=255;
            return;
    }
}

/* Print string using ANSI Shadow font */
void font_ansi_shadow_print(const char *text) {
    if (!text || !*text) return;

    const char *p = text;

    while (*p) {
        const char *segment = p;
        while (*p && *p != '\n') p++;  /* Split by lines */
        size_t seg_len = (size_t)(p - segment);

        /* Draw line row by row */
        for (int row = 0; row < FONT_HEIGHT; row++) {
            for (size_t i = 0; i < seg_len; i++) {
                char ch = segment[i];
                if (ch == '\t') {
                    /* Tab expands into spaces */
                    for (int t = 0; t < FONT_TAB_SIZE; t++) {
                        fputs(char_SPACE[row], stdout);
                        for (int g = 0; g < FONT_GAP_COUNT; g++) {
                            fputs(char_GAP[row], stdout);
                        }
                    }
                } else {
                    const char **glyph = get_glyph(ch);
                    fputs(glyph[row], stdout);
                    /* Add GAP after each glyph except the last */
                    if (i != seg_len - 1) {
                        for (int g = 0; g < FONT_GAP_COUNT; g++) {
                            fputs(char_GAP[row], stdout);
                        }
                    }
                }
            }
            fputc('\n', stdout);
        }

        /* Keep blank line for '\n' in input */
        if (*p == '\n') {
            p++;
            fputc('\n', stdout);
        }
    }
}

/* ===== Gradient print function (真正粒子化渐变) ===== */
void font_ansi_shadow_print_gradient(const char *text, GradientMode mode)
{
    if (!text || !*text) return;

    const char *p = text;
    while (*p) {
        const char *segment = p;
        while (*p && *p != '\n') p++;
        size_t seg_len = (size_t)(p - segment);

        /* Compute max_col */
        size_t total_cols = 0;
        for (size_t i = 0; i < seg_len; i++) {
            char ch = segment[i];
            if (ch == '\t') total_cols += FONT_TAB_SIZE;
            else {
                const char **glyph = get_glyph(ch);
                total_cols += (glyph[0] ? strlen(glyph[0]) : 0);
                total_cols += FONT_GAP_COUNT;
            }
        }

        for (int row = 0; row < FONT_HEIGHT; row++) {
            size_t col_index = 0;
            for (size_t i = 0; i < seg_len; i++) {
                char ch = segment[i];
                if (ch == '\t') {
                    for (int t = 0; t < FONT_TAB_SIZE; t++) {
                        int r,g,b;
                        compute_gradient_color(row, col_index, FONT_HEIGHT, total_cols, mode, &r,&g,&b);
                        set_fg_rgb(r,g,b);
                        fputs(char_SPACE[row], stdout);
                        reset_color();
                        col_index++;
                    }
                } else {
                    const char **glyph = get_glyph(ch);
                    const char *line = glyph[row];
                    size_t glyph_len = strlen(line);

                    /* UTF-8 aware printing */
                    for (size_t c = 0; c < glyph_len; ) {
                        unsigned char chb = (unsigned char)line[c];
                        int char_len = 1;
                        if ((chb & 0xE0) == 0xC0) char_len = 2;
                        else if ((chb & 0xF0) == 0xE0) char_len = 3;
                        else if ((chb & 0xF8) == 0xF0) char_len = 4;

                        int r,g,b;
                        compute_gradient_color(row, col_index, FONT_HEIGHT, total_cols, mode, &r,&g,&b);
                        set_fg_rgb(r,g,b);
                        fwrite(&line[c], 1, char_len, stdout);
                        reset_color();

                        c += char_len;
                        col_index++;
                    }

                    for (int g = 0; g < FONT_GAP_COUNT; g++) {
                        putchar(' ');
                        col_index++;
                    }
                }
            }
            fputc('\n', stdout);
        }
        if (*p == '\n') { p++; fputc('\n', stdout); }
    }
}