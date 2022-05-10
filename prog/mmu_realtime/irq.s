#define INT_STACK_ADDR 0x9000000

.globl irq_vec_asm
irq_vec_asm:
    mov sp, #INT_STACK_ADDR
    sub lr, lr, #4              // Update the lr to where we should jump back to

    push {lr}
    stmfd sp, {r0-r14}^
    sub sp, sp, #60
    mov r0, sp

    bl irq                      // call our actual irq handler

    ldm sp, {r0-r15}^           // restore all registers, jump back.
                                // including r15 also tells ldm to restore spsr
                                // into cpsr
