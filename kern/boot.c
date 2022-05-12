#include <stdint.h>

#include "asm.h"
#include "kern.h"
#include "sys.h"
#include "vm.h"

static pte_1mb_t __attribute__((aligned(1 << 14))) kernel_pagetable[4096];

// all functions in this file go in .text.boot so they exist at low addresses
// (these functions run before the MMU is enabled).
static void __attribute__((section(".text.boot")))
vm_kernel_map(uintptr_t va, uintptr_t pa);
static void __attribute__((section(".text.boot"))) vm_kernel_enable();
static void __attribute__((section(".text.boot")))
dmap_kernel_section(uintptr_t pa);
static void __attribute__((section(".text.boot"))) dmap_kernel_sections();
void __attribute__((section(".text.boot"))) cstart();
extern void __attribute__((section(".text.boot"))) _hlt();

// 1mb
#define SEC_SIZE (1 << 20)

// map va to pa in the kernel pagetable
static void vm_kernel_map(uintptr_t va, uintptr_t pa) {
    pte_1mb_t* pgtbl = (pte_1mb_t*) ka2pa((uintptr_t) kernel_pagetable);

    unsigned pte_idx = va >> 20;
    pte_1mb_t* pte = &pgtbl[pte_idx];

    if (pte->tag != 0b00) {
        _hlt();
    }

    pte->tag = 0b10;
    pte->sec_base_addr = pa >> 20;
}

// double map pa as pa -> pa and pa2ka(pa) -> pa
static void dmap_kernel_section(uintptr_t pa) {
    vm_kernel_map(pa, pa);
    vm_kernel_map(pa2ka(pa), pa);
}

// double map all kernel regions
static void dmap_kernel_sections() {
    // map kernel
    for (uintptr_t pa = 0; pa < MEMSIZE_PHYSICAL; pa += SEC_SIZE) {
        dmap_kernel_section(pa);
    }
    // map uart, gpio, watchdog timer
    dmap_kernel_section(0x20000000);
    dmap_kernel_section(0x20100000);
    dmap_kernel_section(0x20200000);
    sys_clean_and_invalidate_cache();
    sys_invalidate_tlb();
}

// enable the kernel pagetable
static void vm_kernel_enable() {
    sys_invalidate_cache();
    sys_invalidate_tlb();
    dsb();
    sys_set_domain(DOM_MANAGER);
    sys_set_tlb_base(ka2pa((uintptr_t) kernel_pagetable));
    sys_set_cache_control(SYS_MMU_ENABLE | SYS_DCACHE_ENABLE |
                          SYS_ICACHE_ENABLE | SYS_BRANCH_PREDICTION_ENABLE |
                          SYS_WRITE_BUFFER_ENABLE | SYS_MMU_XP);
}

void cstart() {
    extern int _kbss_start, _kbss_end;
    int* bss = (int*) ka2pa((uintptr_t) &_kbss_start);
    int* bss_end = (int*) ka2pa((uintptr_t) &_kbss_end);
    while (bss < bss_end) {
        *bss++ = 0;
    }

    dmap_kernel_sections();
    vm_kernel_enable();

    extern void stack_to_ka();
    stack_to_ka();
    kernel_start();  // shouldn't return
    _hlt();
}
