#pragma once

#include "bits.h"
#include "interrupts.h"
#include "kmalloc.h"
#include "ksan.h"
#include "libc/tinyprintf.h"
#include "vm.h"
#include "sys.h"
#include "uart.h"

void kernel_start();
void reboot();

#define panic(format, args...)  \
    do {                        \
        printf("PANIC: ");      \
        printf(format, ##args); \
        reboot();               \
    } while (0)
