#include <stddef.h>

#include "stdio.h"
#include "trap.h"
#include "csr.h"
#include "csr_func.h"
#include "register.h"
#include "exception.h"
#include "plic.h"
#include "interrupt.h"
#include "timer.h"
#include "utils.h"
#include "memory_manager.h"

extern void trap_handler(void);

static thread_info_t bsp_ti;

bool init_trap(uint64_t cpu_id)
{
    uint64_t kernel_stack_base = (uint64_t)kalloc_4k();
    if(kernel_stack_base == (uint64_t)NULL)
    {
        return false;
    }

    bsp_ti.kernel_stack = kernel_stack_base + 0x1000 - 8;
    bsp_ti.user_stack = (uint64_t)NULL;
    bsp_ti.cpu_id = cpu_id;

    csr_write_sscratch((uint64_t)&bsp_ti);
    write_tp(0);  // clear the thread pointer.

    csr_write_stvec((uint64_t)trap_handler);

    // TODO create and setup thread_info for each processors.

    return true;
}

void enable_interrupt(void)
{
    __asm__ volatile("csrs sstatus, 0x00000002");
    csr_write_sie(-1);
}

void disable_interrupt(void)
{
    __asm__ volatile("csrc sstatus, 0x00000002");
    csr_write_sie(0);
}

void c_trap_handler(const trap_frame_t* trap_frame)
{
    uint64_t code = trap_frame->scause.code;
    if (trap_frame->scause.interrupt) {
        switch (code) {
            case user_software_interrupt:
            case supervisor_software_interrupt:
            case user_timer_interrupt:
            case user_external_interrupt:
                // TODO
                // printf("the trap [%s] is not implemented\n",
                // convert_interrupt_code_to_string(code));
                break;
            case supervisor_timer_interrupt:
            {
                count_up_tick();
                break;
            }
            case supervisor_external_interrupt:
            {
                uint64_t irq = plic_claim();

                bool result = handle_external_interrupt(irq);
                //TODO
                plic_complete(irq);
                //TODO
                break;
            }
            default:
                panic(convert_interrupt_code_to_string(code));
                break;
        }
    }
    else
    {
        //exception
        switch(code)
        {
            default:
                panic(convert_exception_code_to_string(code));
                break;
        }
    }
}
