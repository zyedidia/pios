#pragma once

#include <stdint.h>

#define BASIC_TIMER_IRQ      (1 << 0)
#define BASIC_MAILBOX_IRQ    (1 << 1)
#define BASIC_DOORBELL_0_IRQ (1 << 2)
#define BASIC_DOORBELL_1_IRQ (1 << 3)
#define BASIC_GPU_0_HALTED_IRQ   (1 << 4)
#define BASIC_GPU_1_HALTED_IRQ   (1 << 5)
#define BASIC_ACCESS_ERROR_1_IRQ (1 << 6)
#define BASIC_ACCESS_ERROR_0_IRQ (1 << 7)

void irq_enable(uint32_t val);

typedef enum {
    IRQ_VEC_RESET,
    IRQ_VEC_UNDEF_INSN,
    IRQ_VEC_SOFTWARE_IRQ,
    IRQ_VEC_PREFETCH_ABORT,
    IRQ_VEC_DATA_ABORT,
    IRQ_VEC_UNUSED,
    IRQ_VEC_IRQ,
    IRQ_VEC_FIQ,
} irq_vec_t;

void register_irq_vec(irq_vec_t vec, uintptr_t fn);
uintptr_t get_irq_vec(irq_vec_t vec);

void enable_interrupts();
