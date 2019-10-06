#pragma once

#include <stdint.h>

void csr_write_stvec(uint64_t value);
void csr_write_sie(uint64_t value);
void csr_write_sscratch(uint64_t value);
