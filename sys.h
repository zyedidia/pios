#pragma once

#define CPU_FREQ_MHZ 700

static inline unsigned get_dfar() {
    unsigned dfar = 0;
    asm volatile("mrc p15, 0, %0, c6, c0, 0" : "=r"(dfar));
    return dfar;
}
