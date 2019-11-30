#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "vcpu.h"

typedef struct uart_emulator
{
    uint8_t rhr;
    uint8_t thr;
    uint8_t ier;
    uint8_t fcr;
} uart_emulator_t;

uart_emulator_t *alloc_uart_emulator(void);
bool uart_emulation_store(virtual_cpu_t *vcpu, uint64_t target, uint64_t value,
                          uint8_t width);
bool uart_emulation_load(virtual_cpu_t *vcpu, uint64_t target, uint64_t *value,
                         uint8_t width);
