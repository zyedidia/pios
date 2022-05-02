#include "asm.h"

.section "..boot"

.globl _start
_start:
    ldr pc, _reset_h
    ldr pc, _undef_insn_h
    ldr pc, _software_irq_h
    ldr pc, _prefetch_abort_h
    ldr pc, _data_abort_h
    ldr pc, _unused_h
    ldr pc, _irq_h
    ldr pc, _fiq_h

_reset_h:          .word   _reset
_undef_insn_h:     .word   vec_undef_insn
_software_irq_h:   .word   vec_software_irq
_prefetch_abort_h: .word   vec_prefetch_abort
_data_abort_h:     .word   vec_data_abort
_unused_h:         .word   _reset
_irq_h:            .word   vec_irq
_fiq_h:            .word   vec_fiq

_reset:
	# force the mode to be super
	mov r0, #SUPER_MODE
	# disable interrupts
	orr r0, r0, #(1 << 7)
	msr cpsr, r0
	prefetch_flush(r1)

	# install interrupt vector table at 0x0000
	mov     r0, #0x8000
	mov     r1, #0x0000
	ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
	stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
	ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
	stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}

	mov sp, #STACK_ADDR
	# clear frame pointer
	mov fp, #0
	bl cstart
_hlt:
	b _hlt
