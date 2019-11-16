#include "vmm.h"
#include "csr_func.h"
#include "exception.h"
#include "fdt.h"
#include "vcpu.h"

static void setup_hypervisor_deleg_csr(void)
{
    uint64_t hedeleg = 0;
    uint64_t hideleg = 0;

    // for exception
    // delegate almost exceptions without some environment calls.
    hedeleg |= 1 << instruction_addres_misaligned;
    hedeleg |= 1 << instruction_access_fault;
    hedeleg |= 1 << illegal_instruction;
    hedeleg |= 1 << breakpoint;
    hedeleg |= 1 << load_address_misaligned;
    hedeleg |= 1 << load_access_fault;
    hedeleg |= 1 << store_amo_address_misaligned;
    hedeleg |= 1 << store_amo_access_fault;
    hedeleg |= 1 << environment_call_from_u;
    hedeleg |= 1 << instruction_page_fault;
    hedeleg |= 1 << store_amo_page_fault;

    // for interrupt

    // update
    csr_write_hedeleg(hedeleg);
    csr_write_hideleg(hideleg);
}

bool init_vmm(void)
{
    property_t *prop = get_property("/cpus/cpu@0", "riscv,isa");
    if(!prop)
    {
        return false;
    }

    //TODO check extension 'H' in property.

    setup_hypervisor_deleg_csr();

    return true;
}
