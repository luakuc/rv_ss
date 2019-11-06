#pragma once

// hgatp MODE field for RV64
#define HGATP_MODE_BARE (0)
#define HGATP_MODE_SV39X4 (8)
#define HGATP_MODE_SV48X4 (9)

// for RV64
typedef struct hgatp
{
    union {
        struct
        {
            uint64_t ppn : 44;
            uint16_t vmid : 14;
            uint8_t reserved : 2;
            uint8_t mode : 4;
        } __attribute__((packed));
        uint64_t value;
    };
} hgatp_t;


