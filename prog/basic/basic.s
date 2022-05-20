.section ".text.boot"

.globl start
_start:
	mov r0, #0
	swi 0
