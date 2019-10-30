#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "trap_handler.h"
#include "register.h"

typedef struct scause
{
    uint64_t code : 63;
    uint8_t interrupt : 1;
} __attribute__((packed)) csr_scause_t;

typedef struct thread_info
{
    uint64_t user_stack;
    uint64_t kernel_stack;
    uint64_t cpu_id;
} thread_info_t;

typedef struct trap_frame
{
    uint64_t ra;
    uint64_t gp;
    //uint64_t tp;
    thread_info_t* tp;
    uint64_t t0;
    uint64_t t1;
    uint64_t t2;
    uint64_t s0;
    uint64_t s1;
    uint64_t a0;
    uint64_t a1;
    uint64_t a2;
    uint64_t a3;
    uint64_t a4;
    uint64_t a5;
    uint64_t a6;
    uint64_t a7;
    uint64_t s2;
    uint64_t s3;
    uint64_t s4;
    uint64_t s5;
    uint64_t s6;
    uint64_t s7;
    uint64_t s8;
    uint64_t s9;
    uint64_t s10;
    uint64_t s11;
    uint64_t t3;
    uint64_t t4;
    uint64_t t5;
    uint64_t t6;
    uint64_t sepc;
    uint64_t stval;
    // uint64_t scause;
    csr_scause_t scause;
    uint64_t sstatus;
    uint64_t sscratch;
    uint64_t kernel_sp;
    uint64_t sp;
} trap_frame_t;

bool init_trap(uint64_t cpu_id);

void enable_interrupt(void);
void disable_interrupt(void);
