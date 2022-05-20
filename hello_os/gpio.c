#include <stdbool.h>
#include <stdint.h>

#include "bits.h"
#include "gpio.h"

static volatile uint32_t* gpio_fsel = (uint32_t*) (GPIO_BASE);

void gpio_set_func(unsigned pin, gpio_func_t fn) {
    if (pin >= 32)
        return;
    unsigned off = (pin % 10) * 3;
    unsigned idx = pin / 10;

    uint32_t v = gpio_fsel[idx];
    v &= ~(0b111 << off);
    v |= fn << off;
    gpio_fsel[idx] = v;
}
