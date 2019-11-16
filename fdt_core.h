#pragma once

// FDT spec is included in device tree specification.
// https://www.devicetree.org/specifications/

#include "fdt.h"

#define FDT_HEADER_MAGIC 0xd00dfeed

#define FDT_BEGIN_NODE 0x00000001
#define FDT_END_NODE 0x00000002
#define FDT_PROP 0x00000003
#define FDT_NOP 0x00000004
#define FDT_END 0x00000009

typedef struct fdt_header
{
    uint32_t magic;
    uint32_t totalsize;
    uint32_t off_dt_struct;
    uint32_t off_dt_strings;
    uint32_t off_mem_rsvmap;
    uint32_t version;
    uint32_t last_comp_version;
    uint32_t boot_cpuid_phys;
    uint32_t size_dt_strings;
    uint32_t size_dt_struct;
} fdt_header_t;

typedef struct memory_reserved_entry
{
    uint64_t addres;
    uint64_t size;
} fdt_reserve_entry_t;

typedef uint32_t fdt_struct_entry_t;

typedef struct devicetree
{
    char *name;
    property_t *properties;
    struct devicetree* next;
    struct devicetree *subnodes;
} device_tree_t;

typedef struct property_info
{
    uint32_t len;
    uint32_t nameoff;
} property_info_t;

char *get_next_name(int *index);
char *get_string(int offset);
property_t *parse_property(int *index);
device_tree_t *parse_node(int *index);
