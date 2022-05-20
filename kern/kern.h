#pragma once

#include "libc/tinyprintf.h"

void reboot();
void kernel_start();

#define LOG_MEMSIZE_PHYSICAL 21
#define MEMSIZE_PHYSICAL     (1 << LOG_MEMSIZE_PHYSICAL)

#define PAGESIZE 4096

typedef struct {
    uint8_t page[PAGESIZE];
} page_t;

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
