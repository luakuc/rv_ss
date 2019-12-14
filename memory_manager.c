#include "memory_manager.h"
#include "fdt.h"

// linker script symbols
extern int _heap_start, _heap_end;

static uint64_t heap_base_address;
static uint64_t heap_end_address;

uint64_t dram_base, dram_end;

bool pre_init_memory_manager(void)
{
    heap_base_address = (uint64_t)&_heap_start;
    heap_end_address = heap_base_address + 0x200000; // 2M heap for first step.

    return true;
}

bool post_init_memory_manager(void)
{
    // the value is based on the qemu/virt.
    property_t *prop = get_property("/memory@80000000", "reg");
    if (!prop)
    {
        return false;
    }

    if (prop->len != 4)
    {
        return false;
    }

    uint64_t ram_base = (uint64_t)prop->value[0] << 32 | prop->value[1];
    uint64_t ram_size = (uint64_t)prop->value[2] << 32 | prop->value[3];

    heap_end_address = ram_base + ram_size;

    dram_base = ram_base;
    dram_end = ram_base + ram_size;

    return true;
}

void *kalloc(const size_t size)
{
    // 8byte alignment
    heap_base_address = (heap_base_address + 8 - 1) & -8;

    uint64_t alloc_address = heap_base_address;
    heap_base_address += size;
    if (heap_base_address > heap_end_address)
    {
        return NULL;
    }

    return (void *)alloc_address;
}

void *knalloc_4k(int n)
{
    // 4k alignment
    heap_base_address = (heap_base_address + 0x1000 - 1) & -0x1000;

    uint64_t alloc_address = heap_base_address;
    heap_base_address += 0x1000 * n;

    if (heap_base_address > heap_end_address)
    {
        return NULL;
    }

    return (void *)alloc_address;
}

void *kalloc_4k(void)
{
    return knalloc_4k(1);
}

void *kalloc_16k(void)
{
    // 16k alignment
    heap_base_address = (heap_base_address + 0x4000 - 1) & -0x4000;

    uint64_t alloc_address = heap_base_address;
    heap_base_address += 0x4000;

    if (heap_base_address > heap_end_address)
    {
        return NULL;
    }

    return (void *)alloc_address;
}
