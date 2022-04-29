#include <stdbool.h>
#include <stdint.h>

#include "gpio.h"

static volatile uint32_t* gpio_fsel = (uint32_t*) (GPIO_BASE);

void gpio_set_func(unsigned pin, gpio_func_t fn) {
    if (pin >= 32)
        return;
    unsigned off = (pin % 10) * 3;
    unsigned idx = pin / 10;

    unsigned v = gpio_fsel[idx];
    v &= ~(0b111 << off);
    v |= fn << off;
    gpio_fsel[idx] = v;
}

void gpio_set_output(unsigned pin) {
    gpio_set_func(pin, GPIO_FUNC_OUTPUT);
}

void gpio_set_input(unsigned pin) {
    gpio_set_func(pin, GPIO_FUNC_INPUT);
}

void gpio_set_on(unsigned pin) {
    if (pin >= 32)
        return;
    gpio_set[0] = 1 << pin;
}

void gpio_set_off(unsigned pin) {
    if (pin >= 32)
        return;
    gpio_clr[0] = 1 << pin;
}

void gpio_write(unsigned pin, bool v) {
    if (v) {
        gpio_set_on(pin);
    } else {
        gpio_set_off(pin);
    }
}

bool gpio_read(unsigned pin) {
    if (pin >= 32)
        return false;
    return (gpio_lev[0] >> pin) & 1;
}
