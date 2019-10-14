#include "thread.h"
#include "memory_manager.h"
#include "string.h"
#include "utils.h"
#include "virtual_memory.h"

// for sample code
#include "user_sample.h"

static thread_struct_t boot_thread;
static thread_struct_t test_thread;
static thread_info_t thread_info_1;

void context_switch(context_t *prev, context_t *next);
void return_from_exception(void);

static size_t thread_id = 0;
static size_t get_next_thread_id(void) { return thread_id++; }

void test(void)
{
    while (true)
    {
        __asm__ volatile("wfi");
    }
}

void create_user_process(uint64_t entry_address)
{
    thread_struct_t *new_thread = &test_thread;
    uint64_t thread_stack = (uint64_t)kalloc_4k();
    if (thread_stack == (uint64_t)NULL)
    {
        panic("memory is not enough\n");
    }

    uint64_t kernel_stack = (uint64_t)kalloc_4k();
    if (kernel_stack == (uint64_t)NULL)
    {
        panic("memory is not enough\n");
    }

    uint64_t stack_top = (kernel_stack + 0x1000 - 1) & -8;
    trap_frame_t *trap_frame = (trap_frame_t *)(stack_top - TF_SIZE);
    memory_set(trap_frame, 0x00, sizeof(trap_frame_t));

    trap_frame->sepc = entry_address;
    trap_frame->sp = (thread_stack + 0x1000 - 1) & -8;
    // TODO spp を0にする
    trap_frame->sstatus = 0x8000000000006020;
    //trap_frame->tp = &thread_info_1;
    //trap_frame->tp->user_stack = trap_frame->sp;
    thread_info_1.user_stack = trap_frame->sp;
    thread_info_1.cpu_id = 0;
    trap_frame->sscratch = (uint64_t)&thread_info_1;
    trap_frame->tp = &thread_info_1;

    //trap_frame->tp->kernel_stack = (uint64_t)kernel_stack + 0xff0;

    new_thread->id = get_next_thread_id();
    memory_set(&new_thread->context, 0x00, sizeof(context_t));
    new_thread->context.ra = (uint64_t)return_from_exception;
    new_thread->context.sp = (uint64_t)trap_frame;

    page_table_t page_table = (page_table_t)kalloc_4k();

    memory_copy((void *)page_table, (void *)kernel_root_page_table, 0x1000);
    bool result;
    result =
        virtual_memory_map(page_table, (physical_address_t)_user_sample,
                           (virtual_address_t)_user_sample, 0x1000, 0b11111);
    if (!result)
    {
        // TODO
    }

    result =
        virtual_memory_map(page_table, (physical_address_t)thread_stack,
                           (virtual_address_t)thread_stack, 0x1000, 0b11111);

    new_thread->page_table = page_table;
}

thread_struct_t *create_kernel_thread_test(uint64_t entry_address)
{
    thread_struct_t *new_thread = &test_thread;

    uint64_t thread_stack = (uint64_t)kalloc_4k();
    if (thread_stack == (uint64_t)NULL)
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
    if (kernel_stack == (uint64_t)NULL)
    {
        panic("memory is not enough\n");
    }
    trap_frame->tp->kernel_stack = (uint64_t)kernel_stack + 0xff0;
    trap_frame->tp->cpu_id = 0;

    new_thread->id = get_next_thread_id();
    memory_set(&new_thread->context, 0x00, sizeof(context_t));
    new_thread->context.ra = (uint64_t)return_from_exception;
    new_thread->context.sp = (uint64_t)trap_frame;
    new_thread->page_table = NULL;

    return new_thread;
}

void task_switch(thread_struct_t *prev_thread, thread_struct_t *next_thread)
{
    if (next_thread->page_table)
    {
        write_page_table(next_thread->page_table);
    }

    context_switch(&prev_thread->context, &next_thread->context);
}

void thread_swap_test(void)
{
    static bool is_next_boot_thread = true;
    is_next_boot_thread = !is_next_boot_thread;
    if (is_next_boot_thread)
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
    // create_kernel_thread_test(entry);
    create_user_process((uint64_t)_user_sample);
}
