#pragma once

#include "bits.h"
#include "gpio.h"
#include "libc/tinyprintf.h"
#include "timer.h"
#include "uart.h"
#include "kmalloc.h"
#include "ksan.h"

void kernel_start();
void reboot();

static inline void cache_enable(void) {
    unsigned r;
    asm volatile("MRC p15, 0, %0, c1, c0, 0" : "=r"(r));
    r |= (1 << 12);  // l1 instruction cache
    r |= (1 << 11);  // branch prediction
    asm volatile("MCR p15, 0, %0, c1, c0, 0" ::"r"(r));
}

static inline void cache_disable(void) {
    unsigned r;
    asm volatile("MRC p15, 0, %0, c1, c0, 0" : "=r"(r));
    r &= ~(1 << 12);  // l1 instruction cache
    r &= ~(1 << 11);  // branch prediction
    asm volatile("MCR p15, 0, %0, c1, c0, 0" ::"r"(r));
}

#define panic(format, args...) \
    printf("PANIC: ");         \
    printf(format, ##args);    \
    reboot();
