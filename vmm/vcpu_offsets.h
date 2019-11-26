#pragma once

// for guest
#define VCPU_OFFSET_GUEST_ZERO 0x00
#define VCPU_OFFSET_GUEST_RA 0x08
#define VCPU_OFFSET_GUEST_SP 0x10
#define VCPU_OFFSET_GUEST_GP 0x18
#define VCPU_OFFSET_GUEST_TP 0x20
#define VCPU_OFFSET_GUEST_T0 0x28
#define VCPU_OFFSET_GUEST_T1 0x30
#define VCPU_OFFSET_GUEST_T2 0x38
#define VCPU_OFFSET_GUEST_S0 0x40
#define VCPU_OFFSET_GUEST_S1 0x48
#define VCPU_OFFSET_GUEST_A0 0x50
#define VCPU_OFFSET_GUEST_A1 0x58
#define VCPU_OFFSET_GUEST_A2 0x60
#define VCPU_OFFSET_GUEST_A3 0x68
#define VCPU_OFFSET_GUEST_A4 0x70
#define VCPU_OFFSET_GUEST_A5 0x78
#define VCPU_OFFSET_GUEST_A6 0x80
#define VCPU_OFFSET_GUEST_A7 0x88
#define VCPU_OFFSET_GUEST_S2 0x90
#define VCPU_OFFSET_GUEST_S3 0x98
#define VCPU_OFFSET_GUEST_S4 0xa0
#define VCPU_OFFSET_GUEST_S5 0xa8
#define VCPU_OFFSET_GUEST_S6 0xb0
#define VCPU_OFFSET_GUEST_S7 0xb8
#define VCPU_OFFSET_GUEST_S8 0xc0
#define VCPU_OFFSET_GUEST_S9 0xc8
#define VCPU_OFFSET_GUEST_S10 0xd0
#define VCPU_OFFSET_GUEST_S11 0xd8
#define VCPU_OFFSET_GUEST_T3 0xe0
#define VCPU_OFFSET_GUEST_T4 0xe8
#define VCPU_OFFSET_GUEST_T5 0xf0
#define VCPU_OFFSET_GUEST_T6 0xf8

#define VCPU_OFFSET_GUEST_SEPC 0x100
#define VCPU_OFFSET_GUEST_SSTATUS 0x108
#define VCPU_OFFSET_GUEST_HSTATUS 0x110

// offsets of the host context in struct virtual_cpu_t.
#define VCPU_OFFSET_HOST_RA 0x118
#define VCPU_OFFSET_HOST_SP 0x120
#define VCPU_OFFSET_HOST_GP 0x128
#define VCPU_OFFSET_HOST_TP 0x130
#define VCPU_OFFSET_HOST_T0 0x138
#define VCPU_OFFSET_HOST_T1 0x140
#define VCPU_OFFSET_HOST_T2 0x148
#define VCPU_OFFSET_HOST_S0 0x150
#define VCPU_OFFSET_HOST_S1 0x158
#define VCPU_OFFSET_HOST_A1 0x160
#define VCPU_OFFSET_HOST_A2 0x168
#define VCPU_OFFSET_HOST_A3 0x170
#define VCPU_OFFSET_HOST_A4 0x178
#define VCPU_OFFSET_HOST_A5 0x180
#define VCPU_OFFSET_HOST_A6 0x188
#define VCPU_OFFSET_HOST_A7 0x190
#define VCPU_OFFSET_HOST_S2 0x198
#define VCPU_OFFSET_HOST_S3 0x1a0
#define VCPU_OFFSET_HOST_S4 0x1a8
#define VCPU_OFFSET_HOST_S5 0x1b0
#define VCPU_OFFSET_HOST_S6 0x1b8
#define VCPU_OFFSET_HOST_S7 0x1c0
#define VCPU_OFFSET_HOST_S8 0x1c8
#define VCPU_OFFSET_HOST_S9 0x1d0
#define VCPU_OFFSET_HOST_S10 0x1d8
#define VCPU_OFFSET_HOST_S11 0x1e0
#define VCPU_OFFSET_HOST_T3 0x1e8
#define VCPU_OFFSET_HOST_T4 0x1f0
#define VCPU_OFFSET_HOST_T5 0x1f8
#define VCPU_OFFSET_HOST_T6 0x200

#define VCPU_OFFSET_HOST_SSTATUS 0x208
#define VCPU_OFFSET_HOST_HSTATUS 0x210
#define VCPU_OFFSET_HOST_SSCRATCH 0x218
#define VCPU_OFFSET_HOST_STVEC 0x220

// for virtual CSRs
//#define VCPU_OFFSET_VSSTATUS
//#define VCPU_OFFSET_VSIP
//#define VCPU_OFFSET_VSIE 0x10
//#define VCPU_OFFSET_VSTVEC 0x18
//#define VCPU_OFFSET_VSSCRATCH 0x20
//#define VCPU_OFFSET_VSEPC 0x28
//#define VCPU_OFFSET_VSCAUSE 0x30
//#define VCPU_OFFSET_VSTVAL 0x38
//#define VCPU_OFFSET_VSATP 0x40
