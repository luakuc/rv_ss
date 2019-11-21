#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "memory_map.h"

bool pre_init_memory_manager(void);
bool post_init_memory_manager(void);
void *kalloc(const size_t size);
void *kalloc_4k(void);
void *kalloc_16k(void);
