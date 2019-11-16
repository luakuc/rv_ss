#pragma once

#include <stdint.h>

enum
{
    VIRT_DEBUG,
    VIRT_MROM,
    VIRT_TEST,
    VIRT_CLINT,
    VIRT_PLIC,
    VIRT_UART0,
    VIRT_VIRTIO,
    VIRT_DRAM,
    VIRT_PCIE_MMIO,
    VIRT_PCIE_PIO,
    VIRT_PCIE_ECAM,
    VIRT_MEM_MAP_END,
};

struct memory_map_entry
{
    uintptr_t base;
    uintptr_t size;
};
