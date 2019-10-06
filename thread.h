#pragma once

#include "trap.h"
#include "string.h"
#include <stddef.h>

void return_from_exception(void);

static size_t thread_id = 0;

typedef struct context
{
    uint64_t ra;
    uint64_t sp;
    uint64_t s0;
    uint64_t s1;
    uint64_t s2;
    uint64_t s3;
    uint64_t s4;
    uint64_t s5;
    uint64_t s6;
    uint64_t s7;
    uint64_t s8;
    uint64_t s9;
    uint64_t s10;
    uint64_t s11;

} context_t;

typedef struct thread_struct
{
    uint64_t id;
    context_t context;
} thread_struct_t;


thread_struct_t *create_kernel_thread(uint64_t entry_address);
void task_switch(thread_struct_t *prev_thread, thread_struct_t *next_thread);
