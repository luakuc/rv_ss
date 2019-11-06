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

void csr_write_vsstatus(uint64_t value);
void csr_write_vsip(uint64_t value);
void csr_write_vsie(uint64_t value);
void csr_write_vstvec(uint64_t value);
void csr_write_vsscratch(uint64_t value);
void csr_write_vsepc(uint64_t value);
void csr_write_vscause(uint64_t value);
void csr_write_vstval(uint64_t value);
void csr_write_vsatp(uint64_t value);

void csr_write_hedeleg(uint64_t value);
void csr_write_hideleg(uint64_t value);
void csr_write_hgatp(uint64_t value);
