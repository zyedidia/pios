#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "vm.h"

enum {
    GPIO_TX = 14,
    GPIO_RX = 15,
    GPIO_SDA = 2,
    GPIO_SCL = 3,
};

typedef enum {
    GPIO_FUNC_INPUT = 0,
    GPIO_FUNC_OUTPUT = 1,
    GPIO_FUNC_ALT0 = 4,
    GPIO_FUNC_ALT1 = 5,
    GPIO_FUNC_ALT2 = 6,
    GPIO_FUNC_ALT3 = 7,
    GPIO_FUNC_ALT4 = 3,
    GPIO_FUNC_ALT5 = 2,
} gpio_func_t;

#define GPIO_BASE pa2ka(0x20200000U)

static volatile uint32_t *gpio_set = (uint32_t *) (GPIO_BASE + 0x1C);
static volatile uint32_t *gpio_clr = (uint32_t *) (GPIO_BASE + 0x28);
static volatile uint32_t *gpio_lev = (uint32_t *) (GPIO_BASE + 0x34);

void gpio_set_func(unsigned pin, gpio_func_t fn);
void gpio_set_output(unsigned pin);
void gpio_set_on(unsigned pin);
void gpio_set_off(unsigned pin);
void gpio_set_input(unsigned pin);
void gpio_write(unsigned pin, bool v);
bool gpio_read(unsigned pin);

static inline void gpio_set_on_fast(unsigned pin) {
    gpio_set[0] = 1 << pin;
}

static inline void gpio_set_off_fast(unsigned pin) {
    gpio_clr[0] = 1 << pin;
}

static inline void gpio_write_fast(unsigned pin, bool v) {
    if (v) {
        gpio_set_on_fast(pin);
    } else {
        gpio_set_off_fast(pin);
    }
}

static inline bool gpio_read_fast(unsigned pin) {
    return (gpio_lev[0] >> pin) & 1;
}

void gpio_set_pullup(unsigned pin);
void gpio_set_pulldown(unsigned pin);
void gpio_pud_off(unsigned pin);

enum {
    GPIO_INT0 = 49,
    GPIO_INT1,
    GPIO_INT2,
    GPIO_INT3,
};

int is_gpio_int(unsigned gpio_int);
void gpio_int_rise(unsigned pin);
void gpio_int_fall(unsigned pin);
void gpio_int_rise_async(unsigned pin);
void gpio_int_fall_async(unsigned pin);
int gpio_event_detect(unsigned pin);
void gpio_event_clear(unsigned pin);
