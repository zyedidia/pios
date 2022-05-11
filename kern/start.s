#include "asm.h"

#define prefetch_flush(reg) \
	mov reg, #0;            \
	mcr p15, 0, reg, c7, c5, 4

.section ".boot"

.globl _start
_start:
	# force the mode to be system
	mov r0, #SYS_MODE
	# disable interrupts
	orr r0, r0, #(1 << 7)
	msr cpsr, r0
	prefetch_flush(r1)

	mov sp, #STACK_ADDR
	# clear frame pointer
	mov fp, #0
	bl cstart
_hlt:
	b _hlt

.globl jump_to_ka
jump_to_ka:
	orr lr, lr, #(1 << 31)
	bx lr
