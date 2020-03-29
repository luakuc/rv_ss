#include "smp.h"
#include "register.h"
#include "trap.h"

uint64_t get_cpu_id(void)
{
    thread_local_t *thread_local = (thread_local_t *)read_tp();

    return thread_local->cpu_id;
}
