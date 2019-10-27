#pragma once

#include <stdbool.h>

#include "memory_map.h"

bool init_virtio_mmio(const struct memory_map_entry *memory_map_entry);
