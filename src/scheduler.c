#include "scheduler.h"
#include "thread.h"
#include "csr_func.h"

#include "debug.h"

void wfi_thread(void)
{
    while(true)
    {
        __asm__ volatile("wfi");
    }
}

//TODO move this vriables to thread local storage.
static thread_struct_t *running;
static thread_struct_t *ready_head, *ready_tail;
//static thread_struct_t *sleep_head, *sleep_tail;

bool init_scheduler(void)
{
    running = create_kernel_thread((uintptr_t)wfi_thread);
    if(!running)
    {
        return false;
    }

    ready_head = NULL;
//    sleep_head = NULL;

    return true;
}

void enqueue_ready_list(thread_struct_t* thread)
{
    if(thread == NULL)
    {
        return;
    }

    if(ready_head == NULL)
    {
        ready_head = thread;
        ready_tail = thread;
        thread->next = NULL;
        return;
    }

    thread_struct_t* tmp = ready_tail;
    tmp->next = thread;
    ready_tail = thread;
    thread->next = NULL;
}

thread_struct_t* dequque_ready_list(void)
{
    if(ready_head == NULL)
    {
        return NULL;
    }

    thread_struct_t* head = ready_head;
    ready_head = ready_head->next;

    return head;
}

void schedule(void)
{
    //dump_csrs();
    if(ready_head == NULL)
    {
        return;
    }

    thread_struct_t* prev = running;
    thread_struct_t* next = dequque_ready_list();

    running = next;
    enqueue_ready_list(prev);

    task_switch(prev, next);
}

void kick_off(void)
{
    uint64_t tls = csr_read_sscratch();
    write_tp(tls);
    kick_thread(&running->context);
    //not return
}
