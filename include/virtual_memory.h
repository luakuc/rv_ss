#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct satp
{
    union {
        struct
        {
            uint64_t ppn : 44;  // physical page number
            uint16_t asid : 16; // address space identifire
            uint8_t mode : 4;   // mode
        } __attribute__((packed));
        uint64_t value;
    };
} satp_t;

#define SATP_MODE_BARE 0
#define SATP_MODE_SV39 8
#define SATP_MODE_SV48 9

typedef uint64_t physical_address_t;
typedef uint64_t virtual_address_t;

typedef struct sv39_page_table_entry
{
    union {
        struct
        {
            struct
            {
                uint8_t valid : 1;
                uint8_t readable : 1;
                uint8_t writable : 1;
                uint8_t executable : 1;
                uint8_t user : 1;
                uint8_t global : 1;
                uint8_t accessed : 1;
                uint8_t dirty : 1;
            } __attribute__((packed)) access_control;
            uint8_t rsw : 2; // the supervisor can use this bits.
            uint16_t ppn_0 : 9;
            uint16_t ppn_1 : 9;
            uint32_t ppn_2 : 26;
            uint16_t reserved : 10;
        } __attribute__((packed)) fields;
        uint64_t value;
    };
} page_table_entry_t;

typedef page_table_entry_t *page_table_t;

extern page_table_t kernel_root_page_table;

bool init_virtual_memory();
void write_page_table(const page_table_t page_table);
bool virtual_memory_map(page_table_t page_table, physical_address_t p_address,
                        virtual_address_t v_address, const size_t size,
                        const uint16_t permission);

