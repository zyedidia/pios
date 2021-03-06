#include "asm.h"

#define prefetch_flush(reg) \
	mov reg, #0;            \
	mcr p15, 0, reg, c7, c5, 4

.section ".text.start.boot"

.globl _start
_start:
	# force the mode to be system
	mov r0, #SUPER_MODE
	# disable interrupts
	orr r0, r0, #(1 << 7)
	msr cpsr, r0
	prefetch_flush(r1)

	ldr sp, =kstack_phys
	# clear frame pointer
	mov fp, #0
	bl cstart
.globl _hlt
_hlt:
	b _hlt

.globl stack_to_ka
stack_to_ka:
	ldr sp, =kstack
	bx lr
