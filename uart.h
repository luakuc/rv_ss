#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "memory_map.h"

bool init_uart(const struct memory_map_entry* uart_entry);

void write_char_by_uart(char c);
bool read_char_by_uart(uint8_t* c);
