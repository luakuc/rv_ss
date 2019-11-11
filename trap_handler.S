#include "trap_handler.h"
#include "csr.h"

.globl c_trap_handler
.globl trap_handler
.globl return_from_exception

trap_handler:
    # save previous thread pointer(tp) and load the kernel tp;
    csrrw tp, sscratch, tp
    bnez tp, _save_trap_frame
_trap_from_kernel:
    csrr tp, sscratch
_save_trap_frame:
    sd sp, TP_USER_SP_OFFSET(tp)
    ld sp, TP_KERNEL_SP_OFFSET(tp)
    addi sp, sp, -(TF_SIZE)

    sd ra, TF_RA_OFFSET(sp)
    sd gp, TF_GP_OFFSET(sp)
    sd t0, TF_T0_OFFSET(sp)
    sd t1, TF_T1_OFFSET(sp)
    sd t2, TF_T2_OFFSET(sp)
    sd s0, TF_S0_OFFSET(sp)
    sd s1, TF_S1_OFFSET(sp)
    sd a1, TF_A1_OFFSET(sp)
    sd a0, TF_A0_OFFSET(sp)
    sd a2, TF_A2_OFFSET(sp)
    sd a3, TF_A3_OFFSET(sp)
    sd a4, TF_A4_OFFSET(sp)
    sd a5, TF_A5_OFFSET(sp)
    sd a6, TF_A6_OFFSET(sp)
    sd a7, TF_A7_OFFSET(sp)
    sd s2, TF_S2_OFFSET(sp)
    sd s3, TF_S3_OFFSET(sp)
    sd s4, TF_S4_OFFSET(sp)
    sd s5, TF_S5_OFFSET(sp)
    sd s6, TF_S6_OFFSET(sp)
    sd s7, TF_S7_OFFSET(sp)
    sd s8, TF_S8_OFFSET(sp)
    sd s9, TF_S9_OFFSET(sp)
    sd s10, TF_S10_OFFSET(sp)
    sd s11, TF_S11_OFFSET(sp)
    sd t3, TF_T3_OFFSET(sp)
    sd t4, TF_T4_OFFSET(sp)
    sd t5, TF_T5_OFFSET(sp)
    sd t6, TF_T6_OFFSET(sp)

    csrr s0, sepc
    csrr s1, stval
    csrr s2, scause
    csrr s3, sstatus
    csrr s4, sscratch
    csrr s5, CSR_HSTATUS
    sd s0, TF_SEPC_OFFSET(sp)
    sd s1, TF_STVAL_OFFSET(sp)
    sd s2, TF_SCAUSE_OFFSET(sp)
    sd s3, TF_SSTATUS_OFFSET(sp)
    sd s4, TF_TP_OFFSET(sp)
    sd s5, TF_HSTATUS(sp)

    ld s0, TP_USER_SP_OFFSET(tp)
    sd s0, TF_SP_OFFSET(sp)

    csrw sscratch, x0

    //TODO load the global pointer if you need.

    mv a0, sp

    call c_trap_handler
return_from_exception:

    ld a0, TF_SSTATUS_OFFSET(sp)
    csrw sstatus, a0
    ld a0, TF_SEPC_OFFSET(sp)
    csrw sepc, a0
    ld a0, TF_HSTATUS(sp)
    csrw CSR_HSTATUS, a0

    sd sp, TP_KERNEL_SP_OFFSET(tp)
    csrw sscratch, tp

    ld ra, TF_RA_OFFSET(sp)
    ld gp, TF_GP_OFFSET(sp)
    ld tp, TF_TP_OFFSET(sp)
    ld t0, TF_T0_OFFSET(sp)
    ld t1, TF_T1_OFFSET(sp)
    ld t2, TF_T2_OFFSET(sp)
    ld s0, TF_S0_OFFSET(sp)
    ld s1, TF_S1_OFFSET(sp)
    ld a1, TF_A1_OFFSET(sp)
    ld a0, TF_A0_OFFSET(sp)
    ld a2, TF_A2_OFFSET(sp)
    ld a3, TF_A3_OFFSET(sp)
    ld a4, TF_A4_OFFSET(sp)
    ld a5, TF_A5_OFFSET(sp)
    ld a6, TF_A6_OFFSET(sp)
    ld a7, TF_A7_OFFSET(sp)
    ld s2, TF_S2_OFFSET(sp)
    ld s3, TF_S3_OFFSET(sp)
    ld s4, TF_S4_OFFSET(sp)
    ld s5, TF_S5_OFFSET(sp)
    ld s6, TF_S6_OFFSET(sp)
    ld s7, TF_S7_OFFSET(sp)
    ld s8, TF_S8_OFFSET(sp)
    ld s9, TF_S9_OFFSET(sp)
    ld s10, TF_S10_OFFSET(sp)
    ld s11, TF_S11_OFFSET(sp)
    ld t3, TF_T3_OFFSET(sp)
    ld t4, TF_T4_OFFSET(sp)
    ld t5, TF_T5_OFFSET(sp)
    ld t6, TF_T6_OFFSET(sp)
    ld sp, TF_SP_OFFSET(sp)


    sret
