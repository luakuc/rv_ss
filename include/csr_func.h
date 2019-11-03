#pragma once

#include <stdint.h>

void csr_write_stvec(uint64_t value);
void csr_write_sie(uint64_t value);
void csr_write_sscratch(uint64_t value);
void csr_write_satp(uint64_t satp);

// for CSRs of hypervisor extension
uint64_t csr_read_vsstatus(void);
uint64_t csr_read_vsip(void);
uint64_t csr_read_vsie(void);
uint64_t csr_read_vstvec(void);
uint64_t csr_read_vsscratch(void);
uint64_t csr_read_vsepc(void);
uint64_t csr_read_vscause(void);
uint64_t csr_read_vstval(void);
uint64_t csr_read_vsatp(void);

uint64_t csr_write_vsstatus(void);
uint64_t csr_write_vsip(void);
uint64_t csr_write_vsie(void);
uint64_t csr_write_vstvec(void);
uint64_t csr_write_vsscratch(void);
uint64_t csr_write_vsepc(void);
uint64_t csr_write_vscause(void);
uint64_t csr_write_vstval(void);
uint64_t csr_write_vsatp(void);
