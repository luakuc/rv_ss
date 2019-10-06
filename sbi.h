#pragma once

static inline void sbi_set_timer(uint64_t stime_value)
{
    __asm__ volatile(
            "mv a0, %0\n\t"
            "li a7, 0x0\n\t"
            "ecall\n\t" ::"r"(stime_value) : "a0", "a7"
            );
}
