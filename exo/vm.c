#include "vm.h"
#include "pios.h"

static pde_t* pgdir;

void vm_init() {
    pgdir = kmalloc_aligned(4096 * sizeof(pde_t), 1 << 14);
    memset(pgdir, 0, 4096 * sizeof(pde_t));
}

static void init_second_level(pde_t* pde) {
    pte_small_t* pgtbl = kmalloc_aligned(256 * sizeof(pte_small_t), 1 << 10);
    memset(pgtbl, 0, 256 * sizeof(pte_small_t));
    pde->addr = (uintptr_t) pgtbl >> 10;
    pde->tag = 0b01;
}

void vm_map(uintptr_t va, uintptr_t pa, unsigned flags) {
    unsigned pde_idx = va >> 20;
    pde_t* pde = &pgdir[pde_idx];

    switch (pde->tag) {
        case 0b00:
            init_second_level(pde);
        case 0b01:;
            pte_small_t* pgtbl = (pte_small_t*) (pde->addr << 10);
            pte_small_t* pte = &pgtbl[bits_get(va, 12, 19)];
            pte->addr = pa >> 12;
            pte->ap = AP_RW;
            pte->sz = 1;
            break;
        default:
            panic("invalid pde tag: %d\n", pde->tag);
    }
}

void vm_enable() {
    sys_invalidate_cache();
    sys_invalidate_tlb();
    dsb();
    sys_set_domain(DOM_CLIENT);
    sys_set_tlb_base((uintptr_t) pgdir);
    sys_set_cache_control(SYS_MMU_ENABLE | SYS_DCACHE_ENABLE |
                          SYS_ICACHE_ENABLE | SYS_BRANCH_PREDICTION_ENABLE |
                          SYS_WRITE_BUFFER_ENABLE | SYS_MMU_XP);
}
