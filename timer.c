#include "timer.h"
#include "uart.h"
#include "sbi.h"

#include "thread.h"

#include <stdint.h>

static uint64_t ticks;

uint64_t rdtime(void)
{
    uint64_t value;
    __asm__ volatile("rdtime %0" : "=r"(value));
    return value;
}

bool init_timer(void)
{
    ticks = 0;
    sbi_set_timer(10);
    return true;
}

void count_up_tick(void)
{
    ticks++;
    //TODO context switch
    uint64_t curret_time = rdtime();
    sbi_set_timer(10000000 + curret_time);

    void thread_swap_test(void);
    //thread_swap_test();
}
