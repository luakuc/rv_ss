#include "thread.h"
#include "memory_manager.h"
#include "utils.h"

static thread_struct_t boot_thread;
static thread_struct_t test_thread;
static thread_info_t thread_info_1;

void context_switch(context_t* prev, context_t* next);
void return_from_exception(void);

thread_struct_t *create_kernel_thread_test(uint64_t entry_address)
{
    thread_struct_t *new_thread = &test_thread;

    uint64_t thread_stack = (uint64_t)kalloc_4k();
    if(thread_stack == (uint64_t)NULL)
    {
        panic("memory is not enough\n");
    }
    uint64_t stack_top = (thread_stack + 0x1000 - 1) & -8;
    trap_frame_t *trap_frame = (trap_frame_t *)(stack_top - TF_SIZE);
    memory_set(trap_frame, 0x00, sizeof(trap_frame_t));

    trap_frame->sepc = entry_address;
    trap_frame->sp = stack_top;
    trap_frame->sstatus = 0x8000000000006120;
    trap_frame->tp = &thread_info_1;
    trap_frame->tp->user_stack = (uint64_t)NULL;

    uint64_t kernel_stack = (uint64_t)kalloc_4k();
    if(kernel_stack == (uint64_t)NULL)
    {
        panic("memory is not enough\n");
    }
    trap_frame->tp->kernel_stack = (uint64_t)kernel_stack + 0xff0;
    trap_frame->tp->cpu_id = 0;

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
    static bool is_next_boot_thread = true;
    is_next_boot_thread = !is_next_boot_thread;
    if(is_next_boot_thread)
    {
        task_switch(&test_thread, &boot_thread);
    }
    else
    {
        task_switch(&boot_thread, &test_thread);
    }
}

void init_test_thread(uint64_t entry)
{
    create_kernel_thread_test(entry);
}
