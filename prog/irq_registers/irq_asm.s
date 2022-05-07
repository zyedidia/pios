#define INT_STACK_ADDR 0x9000000

.globl irq_vec_asm
irq_vec_asm:
    mov sp, #INT_STACK_ADDR
    push {lr}                   // Save the PC when we got the interrupt
    stmfd sp!,{r0-r14}^         // Save the other registers (from user regs)
    mov r0, sp                  // record where to access the saved state from
    b irq                       // jump to our actual irq handler

.globl irq_vec_asm_2
irq_vec_asm_2:
    b irq
