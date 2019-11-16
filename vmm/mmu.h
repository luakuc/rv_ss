#pragma once

#include "virtual_memory.h"
#include <stdint.h>

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

page_table_entry_t *page_walk_sv39x4(const page_table_t root,
                                     const virtual_address_t address);

void update_hgatp(page_table_t table);
bool guest_memory_map(page_table_t root, uint64_t guest_physical,
                      uint64_t host_phisical, size_t size, uint16_t permission);

void hfence_gvma(void);
void hfence_bvma(void);
