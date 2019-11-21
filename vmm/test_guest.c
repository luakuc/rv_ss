#include "csr_func.h"
#include "csr_type.h"
#include "exception.h"
#include "io_interface.h"
#include "memory_manager.h"
#include "mmu.h"
#include "string.h"
#include "vcpu.h"

#include <stdbool.h>

// These symbols defined in linker script.
extern int _heap_end, _start;

static void setup_guest_state(virtual_cpu_t *vcpu, uint64_t fdt_base)
{
    vcpu->vcsr.vsatp = 0;
    vcpu->vcsr.vsstatus = 0;
    // an implementaion of OpenSBI for qemu virt sets the  stvec to 0x80200000.
    vcpu->vcsr.vstvec = 0x80200000;

    vcpu_set_pc(vcpu, 0x80200000);
    vcpu_set_sp(vcpu, 0x80100000);
    vcpu->guest_context.a0 = 0; // hart id.
    // TODO
    vcpu->guest_context.a1 = 0x0; // fdt base
}

bool run_test_guest(void)
{
    // size_t kernel_memory_size = &_heap_end - &_start;

    virtual_cpu_t *vcpu = alloc_vcpu();

    setup_guest_state(vcpu);

    bool running = true;
    while (running)
    {
        run_guest(vcpu);

        csr_scause_t scause;
        scause.value = csr_read_scause();
        uint64_t stval = csr_read_stval();

        // interrupt
        if (scause.interrupt)
        {
        }
        // exception
        else
        {
            switch (scause.code)
            {
                case instruction_page_fault:
                case load_page_fault:
                case store_amo_page_fault:
                {
                    // kernel space
                    if(stval >= &_start && stval < &_heap_end)
                    {
                        uint64_t guest_page_base = stval & -0x1000;
                        uint64_t *host_page = kalloc_4k();
                        if (!host_page)
                        {
                            running = false;
                            break;
                        }
                        memory_copy(host_page, (void*)guest_page_base, 0x1000);

                        uint16_t flags = PTE_FLAG_USER | PTE_FLAG_EXEC |
                                         PTE_FLAG_READ | PTE_FLAG_WRITE;
                        guest_memory_map(vcpu->gp_hp_page_table, guest_page_base,
                                         (uint64_t)host_page, 0x1000, flags);
                        break;
                    }
                    // not kenrel space (e.g. firmware, periferal MMIO space)
                    else
                    {
                        //TODO mmio emulation or access fault.
                        running = false;
                        break;
                    }
                }
                default:
                {
                    put_string(convert_exception_code_to_string(scause.code));
                    break;
                }
            }
        }
    }

    return false;
}
