#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void memory_set(void *destination, uint8_t byte, size_t size);
void memory_copy(void *destination, void *source, size_t size);

// void int_to_str(int n, char* buffer);//, int radix);
void int_to_str(uint64_t n, char *buffer); //, int radix)

bool string_compare(const char *str_1, const char *str_2, const size_t size);
size_t string_length(const char *str);

// http://flat-leon.hatenablog.com/entry/cpp_preprocessor
#define _TO_STR(value) #value
#define TO_STR(value) _TO_STR(value)
