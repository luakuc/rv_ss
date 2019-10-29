#pragma once

#include <stdint.h>

void csr_write_stvec(uint64_t value);
void csr_write_sie(uint64_t value);
void csr_write_sscratch(uint64_t value);
void csr_write_satp(uint64_t satp);

// for hypervisor extension
uint64_t csr_read_vsstatus(void);
uint64_t csr_read_vsip(void);
uint64_t csr_read_vsie(void);
uint64_t csr_read_vstvec(void);
uint64_t csr_read_vsscratch(void);
uint64_t csr_read_vsepc(void);
uint64_t csr_read_vscause(void);
uint64_t csr_read_vstval(void);
uint64_t csr_read_vsatp(void);
