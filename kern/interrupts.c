#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "bits.h"
#include "dev.h"
#include "interrupts.h"
#include "kern.h"
#include "sys.h"
#include "vm.h"

typedef struct {
    uint32_t irq_basic_pending;
    uint32_t irq_pending[2];
    uint32_t fiq_control;
    uint32_t enable_irqs[2];
    uint32_t enable_basic_irqs;
    uint32_t disable_irqs[2];
    uint32_t disable_basic_irqs;
} irq_ctrl_t;

static volatile irq_ctrl_t *const irq_ctrl = (irq_ctrl_t *) pa2ka(0x2000b200);

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

void irq_init() {
    irq_ctrl->disable_irqs[0] = 0xffffffff;
    irq_ctrl->disable_irqs[1] = 0xffffffff;
    dev_barrier();

    extern char _interrupt_table;
    uintptr_t addr = (uintptr_t) &_interrupt_table;
    sys_set_vec_base(addr);
}

void irq_init_table(uintptr_t addr) {
    irq_ctrl->disable_irqs[0] = 0xffffffff;
    irq_ctrl->disable_irqs[1] = 0xffffffff;
    dev_barrier();

    extern char _interrupt_table;
    extern char _interrupt_table_end;

    char *dst = (char *) addr;
    char *src = (char *) &_interrupt_table;
    size_t n = &_interrupt_table_end - src;
    memcpy(dst, src, n);

    sys_set_vec_base(addr);
}

extern uintptr_t _vector_table;

static volatile uintptr_t *const vec_tbl = (uintptr_t *) &_vector_table;

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
    printf("~~~~~~~~~~~~~~~~~~~~~~~ PANIC ~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("!!!!!!! IFaulted on 0x%x\n", get_ifar());
    panic_unhandled("prefetch_abort");
}
void __attribute__((interrupt("ABORT"))) vec_data_abort() {
    unsigned lr;
    asm volatile("mov %0, lr" : "=r"(lr));
    printf("~~~~~~~~~~~~~~~~~~~~~~~ PANIC ~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("!!!!!!! DFaulted on 0x%x\n", get_dfar());
    printf("!!!!!!! LR was: %x\n", lr);
    panic_unhandled("data_abort");
}
#include "proc.h"
#include "timer.h"
void vec_irq(regs_t *regs) {
    dev_barrier();
    if (!timer_has_irq()) return;

    proc_scheduler_irq(regs);

    dev_barrier();
    timer_clear_irq();
    dev_barrier();
}
void __attribute__((interrupt("FIQ"))) vec_fiq() {
    panic_unhandled("fiq");
}
