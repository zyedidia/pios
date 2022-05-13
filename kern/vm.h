#pragma once

#include <stdint.h>

#define pa2ka(pa) (((pa) | (1UL << 31)))
#define ka2pa(ka) (((ka) & ~(1UL << 31)))

typedef struct {
    unsigned tag           : 2;
    unsigned b             : 1;
    unsigned c             : 1;
    unsigned xn            : 1;
    unsigned domain        : 4;
    unsigned p             : 1;
    unsigned ap            : 2;
    unsigned tex           : 3;
    unsigned apx           : 1;
    unsigned s             : 1;
    unsigned ng            : 1;
    unsigned super         : 1;
    unsigned _sbz1         : 1;
    unsigned sec_base_addr : 12;
} pte_1mb_t;
_Static_assert(sizeof(pte_1mb_t) == 4, "invalid size for pte_1mb_t");

typedef struct {
    unsigned tag    : 2;
    unsigned sbz1   : 1;
    unsigned ns     : 1;
    unsigned sbz2   : 1;
    unsigned domain : 4;
    unsigned p      : 1;
    unsigned addr   : 22;
} pde_t;
_Static_assert(sizeof(pde_t) == 4, "invalid size for pde_t");

typedef struct {
    unsigned xn   : 1;
    unsigned sz   : 1;
    unsigned b    : 1;
    unsigned c    : 1;
    unsigned ap   : 2;
    unsigned tex  : 3;
    unsigned apx  : 1;
    unsigned s    : 1;
    unsigned ng   : 1;
    unsigned addr : 20;
} pte_small_t;
_Static_assert(sizeof(pte_small_t) == 4, "invalid size for pte_small_t");

typedef struct {
    unsigned sz   : 2;
    unsigned b    : 1;
    unsigned c    : 1;
    unsigned ap   : 2;
    unsigned sbz  : 3;
    unsigned apx  : 1;
    unsigned s    : 1;
    unsigned ng   : 1;
    unsigned tex  : 3;
    unsigned xn   : 1;
    unsigned addr : 16;
} pte_large_t;
_Static_assert(sizeof(pte_large_t) == 4, "invalid size for pte_large_t");

typedef union {
    pde_t pde;
    pte_1mb_t pte_1mb;
} l1pte_t;
_Static_assert(sizeof(l1pte_t) == 4, "invalid size for l1pte_t");

typedef union {
    pte_small_t pte_4k;
    pte_large_t pte_16k;
} l2pte_t;
_Static_assert(sizeof(l2pte_t) == 4, "invalid size for l2pte_t");

typedef struct {
    l1pte_t entries[4096];
} pagetable_t;

enum {
    AP_RW = 0b11,
    AP_NO_ACCESS = 0b00,
    AP_RO = 0b10,
    AP_KER_RW = 0b01,
};

enum {
    DOM_NO_ACCESS = 0b00,  // any access causes fault
    DOM_CLIENT = 0b01,     // client accesses check against perm bits in TLB
    DOM_RESERVED = 0b10,   // client accesses check against perm bits in TLB
    DOM_MANAGER = 0b11,    // TLB access bits are ignored
};

// NOTE: DO NOT CHANGE THESE without changing syscall_list.h
#define SIZE_4KB  (1 << 12)
#define SIZE_16KB (1 << 14)
#define SIZE_1MB  (1 << 20)
#define SIZE_16MB (1 << 24)

typedef enum {
    PAGE_UNMAPPED = 0,
    PAGE_4KB = SIZE_4KB,
    PAGE_16KB = SIZE_16KB,
    PAGE_1MB = SIZE_1MB,
    PAGE_16MB = SIZE_16MB,
} pg_typ_t;

typedef enum {
    RW_USER,
    RW_KER_ONLY,
} protection_typ_t;

pagetable_t *kalloc_pt();
void vm_map(pagetable_t *pt,
            uintptr_t va,
            uintptr_t pa,
            pg_typ_t typ,
            protection_typ_t protection);
void vm_unmap(pagetable_t *pt, uintptr_t va);
void vm_set_pt(pagetable_t *pt);
void vm_flushem();
