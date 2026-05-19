#include <stdio.h>
#include <stdlib.h>
#include "platform.h"

#ifdef _WIN32
    #include <windows.h>
#endif

int platform_init(void) {
#ifdef _WIN32
    system("chcp 65001 > nul");
    SetConsoleOutputCP(CP_UTF8);
    //printf("Running on Windows\n");
#elif __APPLE__
    //printf("Running on macOS\n");
#elif __linux__
    //printf("Running on Linux\n");
#else
    //printf("Unknown platform\n");
#endif
    return 0;
}
