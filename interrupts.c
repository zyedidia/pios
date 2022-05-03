#include "pios.h"
#include "dev.h"

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

static volatile irq_ctrl_t* const irq_ctrl = (irq_ctrl_t*) 0x2000B200;

void irq_enable(uint32_t val) {
    irq_ctrl->enable_basic_irqs = val;
}

static volatile uintptr_t* const irq_tbl = (uintptr_t*) (0x0000 + 8 * 4);

void register_irq_vec(irq_vec_t vec, uintptr_t fn) {
    irq_tbl[vec] = fn;
}

#define panic_unhandled(msg) panic("unhandled interrupt: %s\n", msg)

void __attribute__((interrupt("UNDEF"))) vec_undef_insn() {
    /* panic_unhandled("undef_insn"); */
}
void __attribute__((interrupt("SWI"))) vec_software_irq() {
    /* panic_unhandled("software_irq"); */
}
void __attribute__((interrupt("ABORT"))) vec_prefetch_abort() {
    /* panic_unhandled("prefetch_abort"); */
}
void __attribute__((interrupt("ABORT"))) vec_data_abort() {
    /* panic_unhandled("data_abort"); */
}
void __attribute__((interrupt("IRQ"))) vec_irq() {
    /* panic_unhandled("data_abort"); */
}
void __attribute__((interrupt("FIQ"))) vec_fiq() {
    /* panic_unhandled("fiq"); */
}
