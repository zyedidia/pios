#include <stdint.h>

#include "asm.h"
#include "kern.h"
#include "uart.h"
#include "sys.h"
#include "vm.h"
#include "gpio.h"
#include "interrupts.h"
#include "bits.h"
#include "dev.h"

typedef struct {
    uint32_t irq_basic_pending;
    uint32_t irq_pending[2];
    uint32_t fiq_control;
    uint32_t enable_irqs[2];
    uint32_t enable_basic_irqs;
    uint32_t disable_irqs[2];
    uint32_t disable_basic_irqs;
} irq_ctrl_t;

static volatile irq_ctrl_t* const irq_ctrl = (irq_ctrl_t*) pa2ka(0x2000B200);

void irq_enable_basic(uint32_t irq) {
    irq_ctrl->enable_basic_irqs = bit_set(irq_ctrl->enable_basic_irqs, irq);
}

void irq_enable(uint32_t irq) {
    irq_ctrl->enable_irqs[irq / 32] =
        bit_set(irq_ctrl->enable_irqs[irq / 32], irq % 32);
}

void irq_disable_basic(uint32_t irq) {
    irq_ctrl->disable_basic_irqs = bit_set(irq_ctrl->disable_basic_irqs, irq);
}

void irq_disable(uint32_t irq) {
    irq_ctrl->disable_irqs[irq / 32] =
        bit_set(irq_ctrl->disable_irqs[irq / 32], irq % 32);
}

bool irq_basic_pending(uint32_t irq) {
    return bit_get(irq_ctrl->irq_basic_pending, irq);
}

bool irq_pending(uint32_t irq) {
    return bit_get(irq_ctrl->irq_pending[irq / 32], irq % 32);
}

void irq_init_table() {
    irq_ctrl->disable_irqs[0] = 0xffffffff;
    irq_ctrl->disable_irqs[1] = 0xffffffff;
    dev_barrier();

    extern unsigned _interrupt_table;
    // extern unsigned _interrupt_table_end;

    unsigned high_vec_base = (unsigned)&_interrupt_table;
    printf("Interrupt table is at: %x\n", high_vec_base);
    for (size_t i = 0; i < 6; i++) {
        printf("Interrupt %u does instruction %x\n", i, ((unsigned*)high_vec_base)[i]);
    }
    asm volatile("mcr p15, 0, %0, c12, c0, 0" : : "r"(high_vec_base));
}

extern uintptr_t _vector_table;

static volatile uintptr_t* const vec_tbl = (uintptr_t*) &_vector_table;

void register_irq_vec(irq_vec_t vec, uintptr_t fn) {
    vec_tbl[vec] = fn;
}

#define panic_unhandled(msg) panic("unhandled interrupt: %s\n", msg)

void __attribute__((interrupt("UNDEF"))) vec_undef_insn() {
    panic_unhandled("undef_insn");
}
void __attribute__((interrupt("SWI"))) vec_software_irq() {
    panic_unhandled("software_irq");
}
void __attribute__((interrupt("ABORT"))) vec_prefetch_abort() {
    panic_unhandled("prefetch_abort");
}
void __attribute__((interrupt("ABORT"))) vec_data_abort() {
    panic_unhandled("data_abort");
}
void __attribute__((interrupt("IRQ"))) vec_irq() {
    panic_unhandled("irq");
}
void __attribute__((interrupt("FIQ"))) vec_fiq() {
    panic_unhandled("fiq");
}
