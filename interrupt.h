#pragma once

#include <stdbool.h>
#include <stdint.h>

enum irq_numbers
{
    uart_0 = 0xa,
    //TODO
};

bool handle_external_interrupt(uint64_t irq);
