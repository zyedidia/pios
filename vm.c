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
    pde->domain = DOM_MANAGER;
}

static void system_set_cache_control(unsigned reg) {
    // See section 3.2.7 in arm1176
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r"(reg));
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

#define dsb() asm volatile("mcr p15, #0, %0, c7, c10, #4" : : "r"(0))

#define dmb() asm volatile("mcr p15, #0, %0, c7, c10, #5" : : "r"(0))

static void system_set_domain(unsigned reg) {
    asm volatile("mcr p15, 0, %0, c3, c0, 0" : : "r"(reg));
}

static void system_set_tlb_base(unsigned base) {
    asm volatile("mcr p15, 0, %0, c2, c0, 0" : : "r"(base));
    asm volatile("mcr p15, 0, %0, c2, c0, 1" : : "r"(base));
}

static void system_invalidate_tlb(void) {
    asm volatile("mcr p15, 0, %0, c8, c7, 0" : : "r"(0));
    dsb();
}

static void system_invalidate_cache(void) {
    // See Fig 3.38 in section 3.2.22 in arm1176
    asm volatile("mcr p15, 0, %0, c7, c7, 0" : : "r"(0));
}

void vm_enable() {
    system_invalidate_cache();
    system_invalidate_tlb();
    dsb();
    system_set_domain(DOM_MANAGER);
    system_set_tlb_base((uintptr_t) pgdir);
    system_set_cache_control(
        SYSTEM_MMU_ENABLE | SYSTEM_DCACHE_ENABLE | SYSTEM_ICACHE_ENABLE |
        SYSTEM_BRANCH_PREDICTION_ENABLE | SYSTEM_WRITE_BUFFER_ENABLE);
}
