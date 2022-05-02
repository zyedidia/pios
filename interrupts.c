#include "interrupts.h"

typedef struct {
    uint32_t irq_basic_pending;
    uint32_t irq_pending_1;
    uint32_t irq_pending_2;
    uint32_t fiq_control;
    uint32_t enable_irqs_1;
    uint32_t enable_irqs_2;
    uint32_t enable_basic_irqs;
    uint32_t disable_irqs_1;
    uint32_t disable_irqs_2;
    uint32_t disable_basic_irqs;
} irq_ctrl_t;

#define IRQ_CTRL_BASE 0x2020B200

static volatile irq_ctrl_t* const irq_ctrl = (irq_ctrl_t*) 0x2020B200;

void irq_enable(uint32_t val) {
    irq_ctrl->enable_basic_irqs = val;
}

void __attribute__((interrupt("UNDEF"))) vec_undef_insn() {}
void __attribute__((interrupt("SWI"))) vec_software_irq() {}
void __attribute__((interrupt("ABORT"))) vec_prefetch_abort() {}
void __attribute__((interrupt("ABORT"))) vec_data_abort() {}
void __attribute__((interrupt("IRQ"))) vec_irq() {}
void __attribute__((interrupt("FIQ"))) vec_fiq() {}
