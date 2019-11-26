#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "plic_defs.h"
#include "vcpu.h"

typedef struct plic_emulator
{
    uint32_t priority_registers[SIFIVE_IRQ_NUMBER_MAX];
} plic_emulator_t;

plic_emulator_t *alloc_plic_emulator(void);

bool plic_emulate_store(virtual_cpu_t *vcpu, uint64_t target, uint64_t value,
                        uint8_t width);
