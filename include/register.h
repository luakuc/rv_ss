#pragma once

#include <stdint.h>

//for RV64
#define SSTATUS_UIE (0x1 << 0)
#define SSTATUS_SIE (0x1 << 1)
#define SSTATUS_UPIE (0x1 << 4)
#define SSTATUS_SPIE (0x1 << 5)
#define SSTATUS_SPP (0x1 << 8)
#define SSTATUS_FS (0b11 << 13)
#define SSTATUS_XS (0b11 << 15)
#define SSTATUS_SUM (0x1 << 18)
#define SSTATUS_MXR (0x1 << 19)
#define SSTATUS_UXR (0b11 << 32)

void write_tp(uint64_t value);
uint64_t read_tp(void);
