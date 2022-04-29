#pragma once

#include <stdbool.h>

enum {
    GPIO_TX = 14,
    GPIO_RX = 15,
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

void gpio_set_func(unsigned pin, gpio_func_t fn);
void gpio_set_output(unsigned pin);
void gpio_set_on(unsigned pin);
void gpio_set_off(unsigned pin);
void gpio_set_input(unsigned pin);
void gpio_write(unsigned pin, bool v);
bool gpio_read(unsigned pin);
