#pragma once

#include "pios.h"

#define ns_to_cycles(x) (unsigned) ((x * CPU_FREQ_MHZ) / 1000)

const int compensation = 16;

static inline void write_0(unsigned pin) {
    unsigned start = timer_cycles();
    gpio_write_fast(pin, 1);
    start = delay_ncycles(start, ns_to_cycles(350)-compensation);
    gpio_write_fast(pin, 0);
    delay_ncycles(start, ns_to_cycles(900)-compensation);
}

static inline void write_1(unsigned pin) {
    unsigned start = timer_cycles();
    gpio_write_fast(pin, 1);
    start = delay_ncycles(start, ns_to_cycles(900)-compensation);
    gpio_write_fast(pin, 0);
    delay_ncycles(start, ns_to_cycles(350)-compensation);
}

static inline void flush(unsigned pin) {
    gpio_write(pin, 0);
    delay_us(50);
}

static inline void write_bit(unsigned pin, uint8_t bit) {
    if (bit) {
        write_1(pin);
    } else {
        write_0(pin);
    }
}

static inline void write_byte(unsigned pin, uint8_t byte) {
    // big-endian
    for (int i = 7; i >= 0; i--) {
        write_bit(pin, (byte >> i) & 0x1);
    }
}

static inline void write_rgb(unsigned pin, uint8_t r, uint8_t g, uint8_t b) {
    write_byte(pin, g);
    write_byte(pin, r);
    write_byte(pin, b);
}
