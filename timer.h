#pragma once

#include "sys.h"

static void timer_init() {
    asm volatile("mcr p15, 0, %0, c15, c12, 0" :: "r"(1));
}

static unsigned timer_cycles() {
    unsigned cyc;
    asm volatile("mrc p15, 0, %0, c15, c12, 1" : "=r"(cyc));
    return cyc;
}

static void delay_cycles(unsigned cycles) {
    unsigned rb = timer_cycles();
    while (1) {
        unsigned ra = timer_cycles();
        if ((ra - rb) >= cycles) {
            break;
        }
    }
}

static void delay_us(unsigned us) {
    unsigned rb = timer_cycles();
    while (1) {
        unsigned ra = timer_cycles();
        if ((ra - rb) >= us * (CPU_FREQ_MHZ)) {
            break;
        }
    }
}

static void delay_ms(unsigned ms) {
    delay_us(ms * 1000);
}
