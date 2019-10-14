#pragma once

#include <stdint.h>
#include <stddef.h>

void *memory_set(void *destination, uint8_t byte, size_t size);
void *memory_copy(void *destination, void *source, size_t size);

//void int_to_str(int n, char* buffer);//, int radix);
void int_to_str(uint64_t n, char* buffer); //, int radix)
