#include "vm.h"
#include "bits.h"
#include "kern.h"
#include "kmalloc.h"
#include "sys.h"

pagetable_t *kalloc_pt() {
    pagetable_t *l1pt = (pagetable_t *) kmalloc(sizeof(pagetable_t));
    assert(l1pt);
    memset(l1pt, 0, sizeof(pagetable_t));
    return l1pt;
}

static void init_second_level(pde_t *pde) {
    pte_small_t *pgtbl = kmalloc(256 * sizeof(pte_small_t));
    assert(pgtbl);
    memset(pgtbl, 0, 256 * sizeof(pte_small_t));
    pde->addr = ka2pa((uintptr_t) pgtbl) >> 10;
    pde->tag = 0b01;
}

void vm_map(pagetable_t *pt,
            uintptr_t va,
            uintptr_t pa,
            pg_typ_t typ,
            ap_t prot) {
    unsigned idx = va >> 20;
    l1pte_t *l1pte = &pt->entries[idx];

    // TODO: allow remapping
    assert(l1pte->pde.tag == 0);

    switch (typ) {
    case PAGE_UNMAPPED:
        l1pte->pde.tag = 0b00;
        break;
    case PAGE_1MB:
        l1pte->pte_1mb.tag = 0b10;
        l1pte->pte_1mb.sec_base_addr = pa >> 20;
        l1pte->pte_1mb.ap = prot;
        break;
    case PAGE_4KB:
        if (l1pte->pde.tag == 0b00) {
            init_second_level(&l1pte->pde);
        }
        pte_small_t *l2pt =
            (pte_small_t *) pa2ka((uintptr_t) l1pte->pde.addr << 10);
        pte_small_t *l2pte = &l2pt[bits_get(va, 12, 19)];
        l2pte->addr = pa >> 12;
        l2pte->ap = prot;
        l2pte->sz = 1;
        break;
    default:
        panic("invalid page type: %d\n", typ);
    }
}

void vm_unmap(pagetable_t *pt, uintptr_t va) {
    vm_map(pt, va, 0, PAGE_UNMAPPED, 0);
}

void vm_set_pt(pagetable_t *pt) {
    sys_set_tlb_base(ka2pa((uintptr_t) pt));
    sys_invalidate_tlb();
    sys_clean_and_invalidate_cache();
}

void vm_flushem() {
    sys_clean_and_invalidate_cache();
    sys_flush_btb();
    sys_invalidate_tlb();
    sys_prefetch_flush();
    dsb();
}
