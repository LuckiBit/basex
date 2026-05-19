#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include "version.h"
#include "platform.h"
#include "color.h"
#include "font_ansi_shadow.h"
#include "log.h"

// === Function: Display welcome banner with version info ===
void showBanner() {
    font_ansi_shadow_print_gradient("BASE X", GRADIENT_SPRING);
    printf("\nWelcome to the Base Conversion Tool! ");
    set_fg_color(COLOR_GREEN);
    printf("(v%s)", PROJECT_VERSION_STRING);
    reset_color();
    printf("\nAuthor: ");
    set_fg_color(COLOR_CYAN);
    printf("LuckiBit\n");
    reset_color();
}

// === Function: Convert decimal to binary string ===
void decimalToBinary(uint64_t num, char* binary) {
    int started = 0;
    int index = 0;
    for (int i = 63; i >= 0; i--) {
        char bit = (num & ((uint64_t)1 << i)) ? '1' : '0';
        if (bit == '1') started = 1;
        if (started) {
            binary[index++] = bit;
        }
    }
    if (index == 0) {
        binary[index++] = '0';
    }
    int pad = (8 - (index % 8)) % 8;
    memmove(binary + pad, binary, index);
    for (int i = 0; i < pad; i++) {
        binary[i] = '0';
    }
    binary[index + pad] = '\0';
}

// === Function: Print output results (formatted) ===
void printOutput(double decimal, float fValue, double dValue, uint64_t originalHex) {
    char binary[65];
    decimalToBinary(originalHex, binary);

    set_fg_color(COLOR_YELLOW);
    printf("=== Output Result ===\n");
    reset_color();

    printf("%-22s: %s\n", "Binary", binary);
    printf("%-22s: %.10lf\n", "Decimal", decimal);
    printf("%-22s: 0x%" PRIx64 "\n", "Hexadecimal (integer)", originalHex);

    union { float f; uint32_t i; } uf;
    uf.f = fValue;
    printf("%-22s: %08X\n", "Hexadecimal (float)", uf.i);

    union { double d; uint64_t i; } ud;
    ud.d = dValue;
    printf("%-22s: %016" PRIx64 "\n", "Hexadecimal (double)", ud.i);

    set_fg_color(COLOR_YELLOW);
    printf("=====================\n");
    reset_color();

    // === Also log to file ===
    log_info_file("=== Output Result ===");
    log_info_file("Binary                 : %s", binary);
    log_info_file("Decimal                : %.10lf", decimal);
    log_info_file("Hexadecimal (integer)  : 0x%" PRIx64, originalHex);
    log_info_file("Hexadecimal (float)    : %08X", uf.i);
    log_info_file("Hexadecimal (double)   : %016" PRIx64, ud.i);
    log_info_file("=====================");
}


// === Function: Handle binary input ===
void handleBinaryInput() {
    char binary[65];
    set_fg_color(COLOR_CYAN);
    printf("Enter a binary number: \n> ");
    reset_color();

    set_fg_color(COLOR_GREEN);
    if (scanf("%64s", binary) != 1 || strspn(binary, "01") != strlen(binary)) {
        reset_color();
        set_fg_color(COLOR_RED);
        printf("Invalid input! Please enter a valid binary number.\n");
        reset_color();
        return;
    }
    reset_color();

    uint64_t decimal = 0;
    for (size_t i = 0; i < strlen(binary); i++) {
        decimal = (decimal << 1) | (binary[i] - '0');
    }
    printOutput((double)decimal, (float)decimal, (double)decimal, decimal);
}

// === Function: Handle decimal input ===
void handleDecimalInput() {
    double decimal;
    set_fg_color(COLOR_CYAN);
    printf("Enter a decimal number: \n> ");
    reset_color();

    set_fg_color(COLOR_GREEN);
    if (scanf("%lf", &decimal) != 1) {
        reset_color();
        set_fg_color(COLOR_RED);
        printf("Invalid input! Please enter a valid decimal number.\n");
        reset_color();
        return;
    }
    reset_color();

    uint64_t hex = (uint64_t)decimal;
    printOutput(decimal, (float)decimal, decimal, hex);
}

// === Function: Handle hexadecimal as integer ===
void handleHexAsInteger() {
    unsigned long long hex;
    set_fg_color(COLOR_CYAN);
    printf("Enter a hexadecimal integer (e.g., 0x1A): \n> ");
    reset_color();

    set_fg_color(COLOR_GREEN);
    if (scanf("%llx", &hex) != 1) {
        reset_color();
        set_fg_color(COLOR_RED);
        printf("Invalid input!\n");
        reset_color();
        return;
    }
    reset_color();

    double decimal = (double)hex;
    printOutput(decimal, (float)decimal, decimal, hex);
}

// === Function: Handle hexadecimal as float bit-pattern ===
void handleHexAsFloat() {
    unsigned int hex;
    set_fg_color(COLOR_CYAN);
    printf("Enter a 32-bit float hex (e.g., 40793476): \n> ");
    reset_color();

    set_fg_color(COLOR_GREEN);
    if (scanf("%x", &hex) != 1) {
        reset_color();
        set_fg_color(COLOR_RED);
        printf("Invalid input!\n");
        reset_color();
        return;
    }
    reset_color();

    union { uint32_t i; float f; } u;
    u.i = hex;

    printOutput((double)u.f, u.f, (double)u.f, hex);
}

// === Function: Handle hexadecimal as double bit-pattern ===
void handleHexAsDouble() {
    unsigned long long hex;
    set_fg_color(COLOR_CYAN);
    printf("Enter a 64-bit double hex (e.g., 400F268EC52A411C): \n> ");
    reset_color();

    set_fg_color(COLOR_GREEN);
    if (scanf("%llx", &hex) != 1) {
        reset_color();
        set_fg_color(COLOR_RED);
        printf("Invalid input!\n");
        reset_color();
        return;
    }
    reset_color();

    union { uint64_t i; double d; } u;
    u.i = hex;

    printOutput(u.d, (float)u.d, u.d, hex);
}

// === Main function ===
int main() {
    int choice;
    platform_init();
    log_init();
    showBanner();
    while (1) {
        printf("\nSelect the input data type:\n");
        printf("1. Binary\n");
        printf("2. Decimal\n");
        printf("3. Hexadecimal (as integer)\n");
        printf("4. Hexadecimal (as float)\n");
        printf("5. Hexadecimal (as double)\n");
        printf("0. Exit\n");

        set_fg_color(COLOR_CYAN);
        printf("Enter your choice: ");
        reset_color();

        if (scanf("%d", &choice) != 1) {
            set_fg_color(COLOR_RED);
            printf("Invalid input! Please enter a valid choice.\n");
            reset_color();
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1: handleBinaryInput(); break;
        case 2: handleDecimalInput(); break;
        case 3: handleHexAsInteger(); break;
        case 4: handleHexAsFloat(); break;
        case 5: handleHexAsDouble(); break;
        case 0:
            set_fg_color(COLOR_GREEN);
            printf("Program terminated.\n");
            reset_color();
            log_close();
            return 0;
        default:
            set_fg_color(COLOR_RED);
            printf("Invalid choice! Please try again.\n");
            reset_color();
        }
    }
}
