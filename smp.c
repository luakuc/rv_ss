#include "smp.h"
#include "register.h"
#include "trap.h"

uint64_t get_cpu_id(void)
{
    thread_info_t *thread_info = (thread_info_t *)read_tp();

    return thread_info->cpu_id;
}
