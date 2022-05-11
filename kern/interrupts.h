#pragma once

#include <stdbool.h>
#include <stdint.h>

#define BASIC_TIMER_IRQ          0
#define BASIC_MAILBOX_IRQ        1
#define BASIC_DOORBELL_0_IRQ     2
#define BASIC_DOORBELL_1_IRQ     3
#define BASIC_GPU_0_HALTED_IRQ   4
#define BASIC_GPU_1_HALTED_IRQ   5
#define BASIC_ACCESS_ERROR_1_IRQ 6
#define BASIC_ACCESS_ERROR_0_IRQ 7

void irq_enable_basic(uint32_t irq);
void irq_enable(uint32_t irq);
void irq_disable_basic(uint32_t irq);
void irq_disable(uint32_t irq);

typedef enum {
    IRQ_VEC_UNDEF_INSN,
    IRQ_VEC_SOFTWARE_IRQ,
    IRQ_VEC_PREFETCH_ABORT,
    IRQ_VEC_DATA_ABORT,
    IRQ_VEC_IRQ,
    IRQ_VEC_FIQ,
} irq_vec_t;

void register_irq_vec(irq_vec_t vec, uintptr_t fn);

void irq_init_table(uintptr_t addr);
void irq_init();
void enable_interrupts();
void disable_interrupts();
bool irq_basic_pending(uint32_t irq);
bool irq_pending(uint32_t irq);
