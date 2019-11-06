#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "memory_map.h"

bool init_memory_manager(const struct memory_map_entry memory_map[]);
void *kalloc(const size_t size);
void *kalloc_4k(void);
void *kalloc_16k(void);
