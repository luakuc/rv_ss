#include "csr.h"
#include "vcpu_offsets.h"

.text

.globl switch_to_guest
// void switch_to_guest(virtual_cpu_t* vcpu);
switch_to_guest:
    //save host context
    sd ra, VCPU_OFFSET_HOST_RA(a0)
    sd sp, VCPU_OFFSET_HOST_SP(a0)
    sd gp, VCPU_OFFSET_HOST_GP(a0)
    sd tp, VCPU_OFFSET_HOST_TP(a0)
    sd s0, VCPU_OFFSET_HOST_S0(a0)
    sd s1, VCPU_OFFSET_HOST_S1(a0)
    sd a1, VCPU_OFFSET_HOST_A1(a0)
    sd a2, VCPU_OFFSET_HOST_A2(a0)
    sd a3, VCPU_OFFSET_HOST_A3(a0)
    sd a4, VCPU_OFFSET_HOST_A4(a0)
    sd a5, VCPU_OFFSET_HOST_A5(a0)
    sd a6, VCPU_OFFSET_HOST_A6(a0)
    sd a7, VCPU_OFFSET_HOST_A7(a0)
    sd s2, VCPU_OFFSET_HOST_S2(a0)
    sd s3, VCPU_OFFSET_HOST_S3(a0)
    sd s4, VCPU_OFFSET_HOST_S4(a0)
    sd s5, VCPU_OFFSET_HOST_S5(a0)
    sd s6, VCPU_OFFSET_HOST_S6(a0)
    sd s7, VCPU_OFFSET_HOST_S7(a0)
    sd s8, VCPU_OFFSET_HOST_S8(a0)
    sd s9, VCPU_OFFSET_HOST_S9(a0)
    sd s10, VCPU_OFFSET_HOST_S10(a0)
    sd s11, VCPU_OFFSET_HOST_S11(a0)

    csrr t0, sstatus
    sd t0, VCPU_OFFSET_HOST_SSTATUS(a0)
    csrr t0, CSR_HSTATUS
    sd t0, VCPU_OFFSET_HOST_HSTATUS(a0)
    csrr t0, sscratch
    sd t0, VCPU_OFFSET_HOST_SSCRATCH(a0)
    csrr t0, stvec
    sd t0, VCPU_OFFSET_HOST_STVEC(a0)

    // restore guest contexts
    ld t0, VCPU_OFFSET_GUEST_SEPC(a0)
    csrw sepc, t0
    ld t0, VCPU_OFFSET_GUEST_SSTATUS(a0)
    csrw sstatus, t0
    ld t0, VCPU_OFFSET_GUEST_HSTATUS(a0)
    csrw CSR_HSTATUS, t0

    ld ra, VCPU_OFFSET_GUEST_RA(a0)
    ld sp, VCPU_OFFSET_GUEST_SP(a0)
    ld gp, VCPU_OFFSET_GUEST_GP(a0)
    ld tp, VCPU_OFFSET_GUEST_TP(a0)
    ld s0, VCPU_OFFSET_GUEST_S0(a0)
    ld s1, VCPU_OFFSET_GUEST_S1(a0)
    ld a1, VCPU_OFFSET_GUEST_A1(a0)
    ld a2, VCPU_OFFSET_GUEST_A2(a0)
    ld a3, VCPU_OFFSET_GUEST_A3(a0)
    ld a4, VCPU_OFFSET_GUEST_A4(a0)
    ld a5, VCPU_OFFSET_GUEST_A5(a0)
    ld a6, VCPU_OFFSET_GUEST_A6(a0)
    ld a7, VCPU_OFFSET_GUEST_A7(a0)
    ld s2, VCPU_OFFSET_GUEST_S2(a0)
    ld s3, VCPU_OFFSET_GUEST_S3(a0)
    ld s4, VCPU_OFFSET_GUEST_S4(a0)
    ld s5, VCPU_OFFSET_GUEST_S5(a0)
    ld s6, VCPU_OFFSET_GUEST_S6(a0)
    ld s7, VCPU_OFFSET_GUEST_S7(a0)
    ld s8, VCPU_OFFSET_GUEST_S8(a0)
    ld s9, VCPU_OFFSET_GUEST_S9(a0)
    ld s10, VCPU_OFFSET_GUEST_S10(a0)
    ld s11, VCPU_OFFSET_GUEST_S11(a0)

    csrw sscratch, a0
    ld a0, VCPU_OFFSET_GUEST_A0(a0)

    //TODO setup the stvec to return HS-mode from VS-mode
    // ↑ Is it require?

    // enter the guest
    // pc <- sstatus.sepc
    sret
