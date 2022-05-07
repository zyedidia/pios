#define INT_STACK_ADDR 0x9000000

.globl irq_vec_asm
// TODO: Need to also save cpsr, maybe other state ?
irq_vec_asm:
    mov sp, #INT_STACK_ADDR
    sub lr, lr, #4              // Update the lr to where we should jump back to

    push {lr}                   // Save the PC when we got the interrupt
    stmfd sp!,{r0-r14}^         // Save the other registers (from user regs)

    mov r0, sp                  // record where to access the saved state from
    bl irq                      // call our actual irq handler

    ldm sp!,{r0-r15}^           // restore registers, jump back.
                                // including r15 also tells ldm to restore spsr
                                // into cpsr
