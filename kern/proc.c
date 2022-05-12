#include "kern.h"
#include "proc.h"
#include "vm.h"
#include "kmalloc.h"

static pid_t id;

proc_t procs[NPROC];
proc_t* curproc;

void proc_new(proc_t* proc, uint8_t* code, size_t codesz) {
    proc->id = id++;
    proc->pt = kalloc_pt();
    proc->state = PROC_RUNNABLE;
    proc->code = code;
    proc->codesz = codesz;
    // map kernel into pt
    for (uintptr_t pa = 0; pa < MEMSIZE_PHYSICAL; pa += SIZE_1MB) {
        vm_map(proc->pt, pa2ka(pa), pa, PAGE_1MB);
    }
    // map kernel devices
    vm_map(proc->pt, pa2ka(0x20000000), 0x20000000, PAGE_1MB);
    vm_map(proc->pt, pa2ka(0x20100000), 0x20100000, PAGE_1MB);
    vm_map(proc->pt, pa2ka(0x20200000), 0x20200000, PAGE_1MB);
    // map proc code into pt
    void* pgs = kmalloc(proc->codesz);
    memcpy(pgs, proc->code, proc->codesz);
    for (size_t n = 0; n < proc->codesz; n += SIZE_4KB) {
        vm_map(proc->pt, PROC_ENTRY + n, ka2pa((uintptr_t) pgs + n), PAGE_4KB);
    }
    proc->regs.pc = PROC_ENTRY;
}

void __attribute__((noreturn)) proc_run(proc_t* proc) {
    proc->state = PROC_RUNNING;
    vm_set_pt(proc->pt);
    curproc = proc;

    asm volatile ("ldm %0, {r0-r15}" : : "r"(proc));
    while (1) {}
}
