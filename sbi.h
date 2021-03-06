#pragma once

#include <stdint.h>

// SBI specs: https://github.com/riscv/riscv-sbi-doc/blob/master/riscv-sbi.adoc
static inline void sbi_set_timer(uint64_t stime_value)
{
    __asm__ volatile("mv a0, %0\n\t"
                     "li a7, 0x0\n\t"
                     "ecall\n\t" ::"r"(stime_value)
                     : "a0", "a7");
}

static inline void sbi_shutdown(void)
{
    __asm__ volatile("li a7, 0x8\n\t"
                     "ecall\n\t" ::
                         : "a0");
}

static inline void sbi_console_putchar(char c)
{
    __asm__ volatile(
            "mv a0, %0\n\t"
            "li a7, 0x1\n\t",
            "ecall\n\t" :: "r"(c) : "a0", "a7"
            );
}
