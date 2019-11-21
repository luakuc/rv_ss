#include "memory_manager.h"
#include "vcpu.h"
#include "csr_func.h"
#include "csr_type.h"
#include "exception.h"
#include "io_interface.h"

#include <stdbool.h>

// These symbols defined in linker script.
extern int _heap_end, _start;

static void setup_guest_state(virtual_cpu_t *vcpu)
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
    //size_t kernel_memory_size = &_heap_end - &_start;

    virtual_cpu_t *vcpu = alloc_vcpu();

    setup_guest_state(vcpu);

    while(true)
    {
        run_guest(vcpu);

        csr_scause_t scause;
        scause.value = csr_read_scause();
        //uint64_t stval = csr_read_stval();

        // interrupt
        if(scause.interrupt)
        {

        }
        // exception
        else
        {
            switch(scause.code)
            {
                case instruction_page_fault:
                {
                    //TODO
                    break;
                }
                case load_access_fault:
                {
                    break;
                }
                case store_amo_access_fault:
                {
                    break;
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
