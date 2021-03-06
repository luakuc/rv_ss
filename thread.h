#pragma once

#include <stddef.h>

#include "string.h"
#include "trap.h"
#include "virtual_memory.h"

void return_from_exception(void);

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
    page_table_t page_table;
} thread_struct_t;

thread_struct_t *create_kernel_thread(uint64_t entry_address);
void task_switch(thread_struct_t *prev_thread, thread_struct_t *next_thread);
