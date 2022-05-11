#pragma once

#include "libc/tinyprintf.h"

void reboot();
void kernel_start();

#define panic(format, args...)  \
    do {                        \
        printf("PANIC: ");      \
        printf(format, ##args); \
        reboot();               \
    } while (0)

#define assert(x)                   \
    while (!(x)) {                  \
        printf("ASSERTION FAIL: "); \
        printf("%s\n", #x);         \
        reboot();                   \
    }
