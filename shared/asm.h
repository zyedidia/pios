#pragma once

#define prefetch_flush(reg) \
    mov reg, #0;            \
    mcr p15, 0, reg, c7, c5, 4

#define STACK_ADDR     0x8000000
#define INT_STACK_ADDR 0x9000000

// from A2-2
#define USER_MODE  0b10000
#define FIQ_MODE   0b10001
#define IRQ_MODE   0b10010
#define SUPER_MODE 0b10011
#define ABORT_MODE 0b10111
#define UNDEF_MODE 0b11011
#define SYS_MODE   0b11111
