#pragma once

#include <stdbool.h>

#include "memory_map.h"
#include "plic_defs.h"

bool init_plic(const struct memory_map_entry *plic_entry);
uint64_t plic_claim(void);
void plic_complete(uint64_t irq);
