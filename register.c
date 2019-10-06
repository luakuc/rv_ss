#include "register.h"

void write_tp(uint64_t value)
{
    __asm__ volatile("mv tp, %0" : : "r"(value));
}

uint64_t read_tp(void)
{
    uint64_t value;
    __asm__ volatile("mv %0, tp" : "=r"(value));
    return value;
}
