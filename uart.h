#pragma once

#include <stdint.h>
#include <stdbool.h>

void uart_init(unsigned baud);
bool uart_can_getc();
bool uart_can_putc();
uint8_t uart_getc();
void uart_putc(void* p, char c);
