#pragma once

#include "gpio.h"
#include "libc/tinyprintf.h"
#include "timer.h"
#include "uart.h"

#include "ksan.h"
#include "kalloc.h"

void kernel_start();
void reboot();

static inline void panic(char* msg) {
    printf("PANIC: ");
    printf(msg);
    printf("DONE!!!\n");
    uart_flush_tx();
    reboot();
}
