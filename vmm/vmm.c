#include "vmm.h"
#include "csr_func.h"
#include "endian.h"
#include "exception.h"
#include "fdt.h"
#include "memory_manager.h"
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
    // TODO

    // update
    csr_write_hedeleg(hedeleg);
    csr_write_hideleg(hideleg);
}

bool init_vmm(void)
{
    const char *name = "riscv,isa";
    property_t *prop = get_property("/cpus/cpu@0", name);
    if (!prop)
    {
        return false;
    }
    // The ISA format is "rv64IMA..."
    // More details are in a riscv_isa_string function at
    // QEMU/target/riscv/cpu.c

    char *isa = (char *)kalloc(prop->len * sizeof(uint32_t) + 1);
    if (!isa)
    {
        return false;
    }

    for (int i = 0; i < prop->len; ++i)
    {
        uint32_t tmp = prop->value[i];
        uint32_t str_32 = big2little_32(tmp);

        for (int j = 0; j < 4; ++j)
        {
            size_t index = i * sizeof(uint32_t) + j;
            isa[index] = (str_32 >> (8 * j)) & 0xff;
            if (isa[index] == '\0')
            {
                goto got_isa;
            }
        }
    }

    // cannot find the ISA info from the device tree.
    return false;

got_isa:

{
    // TODO check extension 'H' in property.
    bool exist_h_extention = false;
    for (int i = 0; isa[i]; ++i)
    {
        if (isa[i] == 'h')
        {
            exist_h_extention = true;
        }
    }

    if (!exist_h_extention)
    {
        return false;
    }
}

    setup_hypervisor_deleg_csr();

    return true;
}
