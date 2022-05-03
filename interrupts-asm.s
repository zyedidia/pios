.globl enable_interrupts
enable_interrupts:
	mrs     r0, cpsr
	bic     r0, r0, #(1 << 7)
	msr     cpsr_c, r0
	bx lr
