#include "memory_map.h"

const struct memory_map_entry memory_map[] = {
    [VIRT_DEBUG] = {0x0, 0x100},
    [VIRT_MROM] = {0x1000, 0x11000},
    [VIRT_TEST] = {0x100000, 0x1000},
    [VIRT_CLINT] = {0x2000000, 0x10000},
    [VIRT_PLIC] = {0xc000000, 0x4000000},
    [VIRT_UART0] = {0x10000000, 0x100},
    [VIRT_VIRTIO] = {0x10001000, 0x1000},
    //[VIRT_DRAM] =        { 0x80000000,           0x0 },
    // TODO memory size is hard coded. I have to read device tree.
    [VIRT_DRAM] = {0x80000000, 0x800000}, // 8MiB (size of k210 on chip SRAM)
    [VIRT_PCIE_MMIO] = {0x40000000, 0x40000000},
    [VIRT_PCIE_PIO] = {0x03000000, 0x00010000},
    [VIRT_PCIE_ECAM] = {0x30000000, 0x10000000},
};
