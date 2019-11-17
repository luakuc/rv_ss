#include "memory_manager.h"
#include "vcpu.h"

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
    run_guest(vcpu);

    return false;
}
