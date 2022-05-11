#include <stdint.h>

#include "kern.h"
#include "asm.h"
#include "vm.h"
#include "sys.h"

extern void _hlt();

pte_1mb_t __attribute__((aligned(1 << 14))) early_pagetable[4096];

static void vm_map_early(uintptr_t va, uintptr_t pa) {
    pte_1mb_t* pgtbl = (pte_1mb_t*) ka2pa((uintptr_t) early_pagetable);

    unsigned pte_idx = va >> 20;
    pte_1mb_t* pte = &pgtbl[pte_idx];

    if (pte->tag != 0b00) {
        _hlt();
    }

    pte->tag = 0b10;
    pte->sec_base_addr = pa >> 20;
}

static void vm_unmap_sec(uintptr_t va) {
    pte_1mb_t* pgtbl = (pte_1mb_t*) ka2pa((uintptr_t) early_pagetable);

    unsigned pte_idx = va >> 20;
    pte_1mb_t* pte = &pgtbl[pte_idx];

    if (pte->tag != 0b10) {
        _hlt();
    }

    pte->tag = 0b00;
}

static void map_early_page(uintptr_t pa) {
    vm_map_early(pa, pa);
    vm_map_early(pa2ka(pa), pa);
}

static const unsigned sec_size = (1 << 20); // 1mb

static void map_kernel_pages() {
    // map one mb of stack
    map_early_page(STACK_ADDR - sec_size);
    // and irq stack
    map_early_page(ka2pa(INT_STACK_ADDR - sec_size));
    // map code
    extern char _ktext_start, _ktext_end;
    for (uintptr_t ka = (uintptr_t) &_ktext_start; ka < (uintptr_t) &_ktext_end; ka += sec_size) {
        map_early_page(ka2pa(ka));
    }
    // map uart, gpio, watchdog timer
    map_early_page(0x20000000);
    map_early_page(0x20100000);
    map_early_page(0x20200000);
    sys_clean_and_invalidate_cache();
}

static void unmap_low_pages() {
    // map one mb of stack
    vm_unmap_sec(STACK_ADDR - sec_size);
    // map code
    extern char _ktext_start, _ktext_end;
    for (uintptr_t ka = (uintptr_t) &_ktext_start; ka < (uintptr_t) &_ktext_end; ka += sec_size) {
        vm_unmap_sec(ka2pa(ka));
    }
    // map uart, gpio, watchdog timer
    vm_unmap_sec(0x20000000);
    vm_unmap_sec(0x20100000);
    vm_unmap_sec(0x20200000);
    sys_clean_and_invalidate_cache();
    sys_flush_btb();
    sys_invalidate_tlb();
    sys_prefetch_flush();
    dsb();
}

static void vm_enable_early() {
    sys_invalidate_cache();
    sys_invalidate_tlb();
    dsb();
    sys_set_domain(DOM_MANAGER);
    sys_set_tlb_base(ka2pa((uintptr_t) early_pagetable));
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

    map_kernel_pages();
    vm_enable_early();

    extern void jump_to_ka();
    jump_to_ka();
    unmap_low_pages();

    kernel_start();
    // shouldn't return
    _hlt();
}
