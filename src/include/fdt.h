#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct property
{
    struct property *next;
    uint32_t *value;
    size_t len; // number of words
    char *name;
} property_t;

#include "fdt_core.h"

bool init_fdt(const uint64_t fdt_base);
fdt_header_t* get_fdt_base(void);
property_t *get_property(const char *node_path, const char *prop_name);
