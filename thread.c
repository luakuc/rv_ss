#include "thread.h"

static thread_struct_t boot_thread;
static thread_struct_t test_thread;
static uint8_t thread_1_stack[0x1000];

void context_switch(context_t* prev, context_t* next);
void return_from_exception(void);

thread_struct_t *create_kernel_thread(uint64_t entry_address)
{
    thread_struct_t *new_thread = &test_thread;

    uint64_t stack_top = ((uint64_t)thread_1_stack + 0x1000 - 1) & -8;
    trap_frame_t *trap_frame = (trap_frame_t *)(stack_top - TF_SIZE);
    memory_set(trap_frame, 0x00, sizeof(trap_frame_t));

    trap_frame->sepc = entry_address;
    trap_frame->sp = stack_top;
    trap_frame->sstatus = 0x8000000000007220;

    new_thread->id = thread_id;
    memory_set(&new_thread->context, 0x00, sizeof(context_t));
    new_thread->context.ra = (uint64_t)return_from_exception;
    new_thread->context.sp = (uint64_t)trap_frame;

    thread_id++;

    return new_thread;
}

void task_switch(thread_struct_t *prev_thread, thread_struct_t *next_thread)
{
    context_switch(&prev_thread->context, &next_thread->context);
}

void thread_swap_test(void)
{
    static bool is_next_boot_thread = false;
    if(is_next_boot_thread)
    {
        task_switch(&test_thread, &boot_thread);
    }
    else
    {
        task_switch(&boot_thread, &test_thread);
    }
    is_next_boot_thread = !is_next_boot_thread;
}

void init_test_thread(uint64_t entry)
{
    create_kernel_thread(entry);
}
