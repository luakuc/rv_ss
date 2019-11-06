#include "memory_manager.h"

// linker script symbols
extern int _heap_start, _heap_end;

static uint64_t heap_base_address;
static uint64_t heap_end_address;

bool init_memory_manager(const struct memory_map_entry memory_map[])
{
    heap_base_address = (uint64_t)&_heap_start;
    heap_end_address = (uint64_t)&_heap_end;

    return true;
}

void* kalloc(const size_t size)
{
    uint64_t alloc_address = heap_base_address;
    heap_base_address += size;
    if(heap_base_address > heap_end_address)
    {
        return NULL;
    }

    return (void *)alloc_address;
}

void* kalloc_4k(void)
{
    // 4k alignment
    heap_base_address = (heap_base_address + 0x1000 - 1) & -0x1000;

    uint64_t alloc_address = heap_base_address;
    heap_base_address += 0x1000;

    if(heap_base_address > heap_end_address)
    {
        return NULL;
    }

    return (void *)alloc_address;
}

void *kalloc_16k(void)
{
    heap_base_address = (heap_base_address + 0x4000 - 1) & -0x4000;

    uint64_t alloc_address = heap_base_address;
    heap_base_address += 0x4000;

    if(heap_base_address > heap_end_address)
    {
        return NULL;
    }

    return (void*)alloc_address;
}
