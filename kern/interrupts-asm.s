#include "asm.h"

.globl _interrupt_table
.globl _interrupt_table_end
.align 5
_interrupt_table:
    ldr pc, _reset_h
    ldr pc, _undef_insn_h
    ldr pc, _software_irq_h
    ldr pc, _prefetch_abort_h
    ldr pc, _data_abort_h
    ldr pc, _reset_h
    ldr pc, _irq_h
    ldr pc, _fiq_h

_reset_h:          .word   asm_reset
_undef_insn_h:     .word   asm_undef_insn
_software_irq_h:   .word   asm_software_irq
_prefetch_abort_h: .word   asm_prefetch_abort
_data_abort_h:     .word   asm_data_abort
_irq_h:            .word   asm_irq
_fiq_h:            .word   asm_fiq
_interrupt_table_end:

asm_reset:
    ldr sp, =kstack
    bl reboot
asm_undef_insn:
    ldr sp, =kstack
    ldr pc, _vec_undef_insn
asm_software_irq:
    ldr sp, =kstack
    // sub lr, lr, #4
    push {lr}
    stmfd sp, {r0-r14}^
    sub sp, sp, #60
    mov r0, sp
    bl syscall
    ldm sp, {r0-r15}^
asm_prefetch_abort:
    ldr sp, =kstack
    ldr pc, _vec_prefetch_abort
asm_data_abort:
    ldr sp, =kstack
    ldr pc, _vec_data_abort
asm_irq:
    ldr sp, =kstack
    sub lr, lr, #4
    push {lr}
    stmfd sp, {r0-r14}^
    sub sp, sp, #60
    mov r0, sp
    ldr r1, _vec_irq
    blx r1
    ldm sp, {r0-r15}^
asm_fiq:
    ldr sp, =kstack
    ldr pc, _vec_fiq

.globl _vector_table
_vector_table:
_vec_undef_insn:     .word vec_undef_insn
_vec_software_irq:   .word vec_software_irq
_vec_prefetch_abort: .word vec_prefetch_abort
_vec_data_abort:     .word vec_data_abort
_vec_irq:            .word vec_irq
_vec_fiq:            .word vec_fiq

.globl enable_interrupts
enable_interrupts:
    mrs r0, cpsr
    bic r0, r0, #(1 << 7)
    msr cpsr_c, r0
    bx lr

.globl disable_interrupts
disable_interrupts:
    mrs r0, cpsr
    orr r0, r0, #(1 << 7)
    msr cpsr_c, r0
    bx lr
