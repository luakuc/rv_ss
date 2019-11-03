#include "csr_func.h"

void csr_write_stvec(uint64_t value)
{
    __asm__ volatile("csrw stvec, %0" ::"r"(value));
}

void csr_write_sie(uint64_t value)
{
    __asm__ volatile("csrw sie, %0" ::"r"(value));
}

void csr_write_sscratch(uint64_t value)
{
    __asm__ volatile("csrw sscratch, %0" ::"r"(value));
}

void csr_write_satp(uint64_t satp)
{
    __asm__ volatile("csrw satp, %0" ::"r"(satp));
}

uint64_t csr_read_vsstatus(void)
{
    uint64_t vsstatus;
    // vsstatus : 0x200
    __asm__ volatile("csrr %0, 0x200" : "=r"(vsstatus));
    return vsstatus;
}

uint64_t csr_read_vsip(void)
{
    uint64_t vsip;
    __asm__ volatile("csrr %0, 0x244" : "=r"(vsip));
    return vsip;
}

uint64_t csr_read_vsie(void)
{
    uint64_t vsie;
    __asm__ volatile("csrr %0, 0x204" : "=r"(vsie));
    return vsie;
}

uint64_t csr_read_vstvec(void)
{
    uint64_t vstvec;
    __asm__ volatile("csrr %0, 0x205" : "=r"(vstvec));
    return vstvec;
}

uint64_t csr_read_vsscratch(void)
{
    uint64_t vsscratch;
    __asm__ volatile("csrr %0, 0x240" : "=r"(vsscratch));
    return vsscratch;
}

uint64_t csr_read_vsepc(void)
{
    uint64_t vsepc;
    __asm__ volatile("csrr %0, 0x241" : "=r"(vsepc));
    return vsepc;
}

uint64_t csr_read_vscause(void)
{
    uint64_t vscause;
    __asm__ volatile("csrr %0, 0x242" : "=r"(vscause));
    return vscause;
    // 0x242
}

uint64_t csr_read_vstval(void)
{
    uint64_t vstval;
    __asm__ volatile("csrr %0, 0x243" : "=r"(vstval));
    return vstval;
}

uint64_t csr_read_vsatp(void)
{
    uint64_t vscause;
    __asm__ volatile("csrr %0, 0x280" : "=r"(vscause));
    return vscause;
}

uint64_t csr_write_vsstatus(void)
{
    uint64_t vsstatus;
    // vsstatus : 0x200
    __asm__ volatile("csrw 0x200, %0" : : "r"(vsstatus));
    return vsstatus;
}

uint64_t csr_write_vsip(void)
{
    uint64_t vsip;
    __asm__ volatile("csrw 0x244, %0" : : "r"(vsip));
    return vsip;
}

uint64_t csr_write_vsie(void)
{
    uint64_t vsie;
    __asm__ volatile("csrw 0x204, %0" : : "r"(vsie));
    return vsie;
}

uint64_t csr_write_vstvec(void)
{
    uint64_t vstvec;
    __asm__ volatile("csrw 0x205, %0" : : "r"(vstvec));
    return vstvec;
}

uint64_t csr_write_vsscratch(void)
{
    uint64_t vsscratch;
    __asm__ volatile("csrw 0x240, %0" : : "r"(vsscratch));
    return vsscratch;
}

uint64_t csr_write_vsepc(void)
{
    uint64_t vsepc;
    __asm__ volatile("csrw 0x241, %0" : : "r"(vsepc));
    return vsepc;
}

uint64_t csr_write_vscause(void)
{
    uint64_t vscause;
    __asm__ volatile("csrw 0x280, %0" : : "r"(vscause));
    return vscause;
}

uint64_t csr_write_vstval(void)
{
    uint64_t vstval;
    __asm__ volatile("csrw 0x243, %0" : : "r"(vstval));
    return vstval;
}

uint64_t csr_write_vsatp(void)
{
    uint64_t vscause;
    __asm__ volatile("csrw 0x280, %0" : : "r"(vscause));
    return vscause;
}
