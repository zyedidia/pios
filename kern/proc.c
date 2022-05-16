#include "bits.h"
#include "proc.h"
#include "kern.h"
#include "kmalloc.h"
#include "vm.h"

static pid_t id;

proc_t procs[NPROC];
proc_t *curproc;

proc_t *proc_new(uint8_t *code_start, uint8_t *code_end) {
    size_t codesz = code_end - code_start;

    proc_t *proc = &procs[id];
    proc->id = id++;
    proc->pt = kalloc_pt();
    proc->state = PROC_RUNNABLE;
    proc->code = code_start;
    proc->codesz = codesz;
    // map kernel into pt
    for (uintptr_t pa = 0; pa < MEMSIZE_PHYSICAL; pa += SIZE_1MB) {
        vm_map(proc->pt, pa2ka(pa), pa, PAGE_1MB, AP_KER_RW);
    }
    // map kernel devices
    // TODO(masot) protect things
    vm_map(proc->pt, pa2ka(0x20000000), 0x20000000, PAGE_1MB, AP_RW);
    vm_map(proc->pt, pa2ka(0x20100000), 0x20100000, PAGE_1MB, AP_RW);
    vm_map(proc->pt, pa2ka(0x20200000), 0x20200000, PAGE_1MB, AP_RW);
    // map proc code into pt
    void *pgs = kmalloc(proc->codesz);
    memcpy(pgs, proc->code, proc->codesz);
    for (size_t n = 0; n < proc->codesz; n += SIZE_4KB) {
        vm_map(proc->pt, PROC_ENTRY + n, ka2pa((uintptr_t) pgs + n), PAGE_4KB,
               AP_RW);
    }
    proc->regs.pc = PROC_ENTRY;
    return proc;
}

#include "asm.h"
#include "sys.h"
void __attribute__((noreturn)) proc_run(proc_t *proc) {
    proc->state = PROC_RUNNING;
    vm_set_pt(proc->pt);
    curproc = proc;

    // NOTE: To use SPSR, need to be sure we are *NOT* in user/system mode.
    // TODO(masot): add an assert like that
    set_spsr(bits_set(get_cpsr(), 0, 4, 0b10000));
    asm volatile("ldm %0, {r0-r15}^" : : "r"(proc));
    while (1) {
    }
}

void swippityswap(regs_t *live_state, proc_t *new_thread, proc_t *old_thread) {
    memcpy(&old_thread->regs, live_state, sizeof(regs_t));
    memcpy(live_state, &new_thread->regs, sizeof(regs_t));
}

void proc_scheduler_irq(regs_t *regs) {
    pid_t curr_pid = curproc->id, next_pid = (curr_pid + 1) % id;

    swippityswap(regs, &procs[next_pid], &procs[curr_pid]);

    curproc->state = PROC_RUNNABLE;
    curproc = &procs[next_pid];
    curproc->state = PROC_RUNNING;
    vm_set_pt(curproc->pt);
}
