#pragma once

#include <stdint.h>


typedef struct virtual_cpu
{
    struct
    {
        uint64_t ra;
        uint64_t sp;
        uint64_t gp;
        uint64_t tp;
        uint64_t s0;
        uint64_t s1;
        // host a0 register is stored in sstatus.
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

        uint64_t sstatus;
        uint64_t hstatus;
        uint64_t sscratch;
        uint64_t stvec;
    } host_context;

    struct
    {
        uint64_t ra;
        uint64_t sp;
        uint64_t gp;
        uint64_t tp;
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

        uint64_t sepc;
        uint64_t sstatus;
        uint64_t hstatus;
    } guest_context;

    // CSRs for V mode (Guest)
    struct
    {
        uint64_t vsstatus;
        uint64_t vsip;
        uint64_t vsie;
        uint64_t vstvec;
        uint64_t vsscratch;
        uint64_t vsepc;
        uint64_t vscause;
        uint64_t vstval;
        uint64_t vsatp;
    } vcsr;

    // TODO use virtual_address_t and physical_address_t.
    hgatp_t hgatp;
    uint64_t hgatp_phy;

} virtual_cpu_t;

virtual_cpu_t *alloc_vcpu(void);
void vcpu_set_pc(virtual_cpu_t* vcpu, uint64_t pc);
void run_guest(virtual_cpu_t* vcpu);
