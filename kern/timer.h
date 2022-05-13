#pragma once

#include "sys.h"
#include <stdbool.h>
#include <stdint.h>

static inline void timer_init() {
    asm volatile("mcr p15, 0, %0, c15, c12, 0" ::"r"(1));
}

static inline unsigned timer_cycles() {
    unsigned cyc;
    asm volatile("mrc p15, 0, %0, c15, c12, 1" : "=r"(cyc));
    return cyc;
}

static inline unsigned delay_ncycles(unsigned rb, unsigned n) {
    while (1) {
        unsigned ra = timer_cycles();
        if ((ra - rb) >= n) {
            return ra;
        }
    }
}

static inline void delay_us(unsigned us) {
    unsigned rb = timer_cycles();
    while (1) {
        unsigned ra = timer_cycles();
        if ((ra - rb) >= us * (CPU_FREQ_MHZ)) {
            break;
        }
    }
}

static inline void delay_ms(unsigned ms) {
    delay_us(ms * 1000);
}

typedef struct {
    uint32_t load;
    uint32_t value;
    uint32_t ctrl;
    uint32_t irq_clear;
    uint32_t raw_irq;
    uint32_t masked_irq;
    uint32_t reload;
    uint32_t pre_divider;
    uint32_t free_running_counter;
} my_timer_t;

#define TIMER_CTRL_32BIT (1 << 1)

#define TIMER_CTRL_PRESCALE_1   (0 << 2)
#define TIMER_CTRL_PRESCALE_16  (1 << 2)
#define TIMER_CTRL_PRESCALE_256 (2 << 2)

#define TIMER_CTRL_INT_ENABLE  (1 << 5)
#define TIMER_CTRL_INT_DISABLE (0 << 5)

#define TIMER_CTRL_ENABLE  (1 << 7)
#define TIMER_CTRL_DISABLE (0 << 7)

static volatile my_timer_t* const timer = (my_timer_t*) pa2ka(0x2000b400);

static void timer_irq_load(uint32_t load) {
    timer->load = load;
    timer->ctrl = TIMER_CTRL_32BIT | TIMER_CTRL_ENABLE | TIMER_CTRL_INT_ENABLE |
                  TIMER_CTRL_PRESCALE_256;
}

static bool timer_has_irq() {
    return timer->masked_irq;
}

static void timer_clear_irq() {
    timer->irq_clear = 1;
}
