#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "vcpu.h"
#include "virtual_memory.h"

#define RV64_LOAD_STORE_WIDTH_BYTE (0b00)
#define RV64_LOAD_STORE_WIDTH_HALF (0b01)
#define RV64_LOAD_STORE_WIDTH_WORD (0b10)
#define RV64_LOAD_STORE_WIDTH_LONG (0b11)

// rv64 load and store instruction emulation
bool instruction_emulation(virtual_cpu_t *vcpu, uint64_t instruction_address);
bool read_guest_instuction(virtual_cpu_t* vcpu, uint64_t address, uint32_t* instruction);
