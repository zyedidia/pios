#include "proc.h"
#include "vm.h"

static pid_t id;

proc_t procs[NPROC];
proc_t* curproc;

void proc_new(proc_t* proc) {
    *proc = (proc_t) {
        .id = id++,
        .pt = kalloc_pt(),
        .state = PROC_RUNNABLE,
    };
    proc->regs.pc = PROC_ENTRY;
}

void __attribute__((noreturn)) proc_run(proc_t* proc) {
    proc->state = PROC_RUNNING;
    vm_set_pt(proc->pt);
    curproc = proc;

    asm volatile ("ldm %0, {r0-r15}" : : "r"(proc));
    while (1) {}
}
