#pragma once
#include <stdint.h>
#include <stdbool.h>

#include "plic_defs.h"
#include "vcpu.h"

typedef struct plic_emulator
{
    uint32_t priority_registers[SIFIVE_IRQ_NUMBER_MAX];
} plic_emulator_t;

plic_emulator_t *alloc_plic_emulator(void);
bool plic_emulate(virtual_cpu_t *vcpu, uint64_t target,
                       uint64_t instruction_address, bool is_read);

