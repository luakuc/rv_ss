#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "virtual_memory.h"

// rv64 load and store instruction emulation
bool instruction_emu_store(const page_table_t page_table,
                           uint64_t instruction_address);

