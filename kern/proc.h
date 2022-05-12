#pragma once

#include <stdint.h>

#include "vm.h"

#define PROC_ENTRY 0x8000
#define NPROC 16

typedef uint32_t pid_t;

typedef struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t sp;
    uint32_t lr;
    uint32_t pc;
} regs_t;

typedef enum {
    PROC_FREE,
    PROC_RUNNABLE,
    PROC_RUNNING,
} proc_state_t;

typedef struct {
    regs_t regs;
    pid_t id;
    pagetable_t* pt;

    proc_state_t state;
} proc_t;
