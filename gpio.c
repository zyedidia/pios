#include <stdbool.h>
#include <stdint.h>

#include "interrupts.h"
#include "bits.h"
#include "timer.h"
#include "gpio.h"

static volatile uint32_t* gpio_fsel = (uint32_t*) (GPIO_BASE);
static volatile uint32_t* gpio_eds = (uint32_t*) (GPIO_BASE + 0x40);
static volatile uint32_t* gpio_ren = (uint32_t*) (GPIO_BASE + 0x4c);
static volatile uint32_t* gpio_fen = (uint32_t*) (GPIO_BASE + 0x58);
static volatile uint32_t* gpio_aren = (uint32_t*) (GPIO_BASE + 0x7c);
static volatile uint32_t* gpio_afen = (uint32_t*) (GPIO_BASE + 0x88);
static volatile uint32_t* gpio_pud = (uint32_t*) (GPIO_BASE + 0x94);
static volatile uint32_t* gpio_pudclk = (uint32_t*) (GPIO_BASE + 0x98);

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

enum {
    PUD_OFF = 0b00,
    PUD_PULLDOWN = 0b01,
    PUD_PULLUP = 0b10,
};

static void gpio_apply_pud(unsigned pin) {
    delay_ncycles(timer_cycles(), 200);
    gpio_pudclk[0] = (1 << pin);
    delay_ncycles(timer_cycles(), 200);
    gpio_pud[0] = 0;
    gpio_pudclk[0] = 0;
}

void gpio_set_pullup(unsigned pin) {
    if (pin >= 32)
        return;
    gpio_pud[0] = PUD_PULLUP;
    gpio_apply_pud(pin);
}

void gpio_set_pulldown(unsigned pin) {
    if (pin >= 32)
        return;
    gpio_pud[0] = PUD_PULLDOWN;
    gpio_apply_pud(pin);
}

void gpio_pud_off(unsigned pin) {
    if (pin >= 32)
        return;
    gpio_pud[0] = PUD_OFF;
    gpio_apply_pud(pin);
}

void gpio_int_rise(unsigned pin) {
    if (pin >= 32)
        return;
    gpio_ren[0] = bit_set(gpio_ren[0], pin);
    irq_enable(GPIO_INT3);
}

void gpio_int_fall(unsigned pin) {
    if (pin >= 32)
        return;
    gpio_fen[0] = bit_set(gpio_fen[0], pin);
    irq_enable(GPIO_INT3);
}

void gpio_int_rise_async(unsigned pin) {
    if (pin >= 32)
        return;
    gpio_aren[0] = bit_set(gpio_aren[0], pin);
    irq_enable(GPIO_INT3);
}

void gpio_int_fall_async(unsigned pin) {
    if (pin >= 32)
        return;
    gpio_afen[0] = bit_set(gpio_afen[0], pin);
    irq_enable(GPIO_INT3);
}

int gpio_event_detect(unsigned pin) {
    if (pin >= 32)
        return 0;
    return bit_get(gpio_eds[0], pin);
}

void gpio_event_clear(unsigned pin) {
    if (pin >= 32)
        return;
    gpio_eds[0] = bit_set(gpio_eds[0], pin);
}
