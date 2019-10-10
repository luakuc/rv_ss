#include "csr_func.h"

void csr_write_stvec(uint64_t value)
{
    __asm__ volatile("csrw stvec, %0" :: "r"(value));
}

void csr_write_sie(uint64_t value)
{
    __asm__ volatile("csrw sie, %0" :: "r"(value));
}

void csr_write_sscratch(uint64_t value)
{
    __asm__ volatile("csrw sscratch, %0" ::"r"(value));
}

void csr_write_satp(uint64_t satp)
{
    __asm__ volatile("csrw satp, %0" :: "r"(satp));
}
