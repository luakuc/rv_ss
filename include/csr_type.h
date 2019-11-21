#pragma once

#include <stdint.h>

typedef union scause
{
    struct {
        uint64_t code : 63;
        uint8_t interrupt : 1;
    } __attribute__((packed));
    uint64_t value;
} __attribute__((packed)) csr_scause_t;
