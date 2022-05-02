.globl _enable_interrupts
_enable_interrupts:
	mrs     r0, cpsr
	bic     r0, r0, #(1 << 7)
	msr     cpsr_c, r0
	cpsie   i

	mov     pc, lr
