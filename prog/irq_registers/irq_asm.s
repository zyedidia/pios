#define INT_STACK_ADDR 0x9000000

.globl irq_vec_asm
// TODO: Need to also save cpsr, maybe other state ?
irq_vec_asm:
    mov sp, #INT_STACK_ADDR
    sub lr, lr, #4              // Update the lr to where we should jump back to

    push {lr}                   // Save the PC when we got the interrupt
    sub sp, sp, #8              // make space for user-mode sp, lr
    push {r0-r12}               // Save the unbanked registers (NOTE: don't use
                                // FIQ mode!)

    mov r0, sp                  // save the location of where the saved state
                                // starts.

    mrs r1, cpsr                // r1 = current state
    orr r2, r1, #0x1f           // r2 = system mode
    msr cpsr, r2                // set the new mode (rmw)

    str r14, [r0, #52]          // store the sp
    str r13, [r0, #56]          // store the lr

    msr cpsr, r1                // reset the mode

    bl irq                      // call our actual irq handler

    ldm sp, {r0-r15}^           // restore all registers, jump back.
                                // including r15 also tells ldm to restore spsr
                                // into cpsr
