#pragma once

#include <stdint.h>

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

enum {
    AP_RW = 0b11,
    AP_NO_ACCESS = 0b00,
    AP_RO = 0b10,
};

enum {
    DOM_NO_ACCESS = 0b00,  // any access causes fault
    DOM_CLIENT = 0b01,     // client accesses check against perm bits in TLB
    DOM_RESERVED = 0b10,   // client accesses check against perm bits in TLB
    DOM_MANAGER = 0b11,    // TLB access bits are ignored
};

void vm_init();
void vm_map(uintptr_t va, uintptr_t pa, unsigned flags);
void vm_enable();

#define MMU_PAGE_SECTION    0x00002  // 1MB sections
#define MMU_PAGE_CACHEABLE  0x00008  // cache enabled
#define MMU_PAGE_BUFFERABLE 0x00004  // write-buffer enabled

#define SYSTEM_MMU_ENABLE               (1 << 0)
#define SYSTEM_DCACHE_ENABLE            (1 << 2)
#define SYSTEM_WRITE_BUFFER_ENABLE      (1 << 3)
#define SYSTEM_BRANCH_PREDICTION_ENABLE (1 << 11)
#define SYSTEM_ICACHE_ENABLE            (1 << 12)
#define SYSTEM_ROUND_ROBIN_ENABLE       (1 << 14)
