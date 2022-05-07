#include "asm.h"

.section ".boot"

.globl _start
_start:
	# force the mode to be super
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
