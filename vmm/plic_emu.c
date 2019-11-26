#include "plic_emu.h"
#include "inst_emu.h"
#include "memory_manager.h"
#include "plic_defs.h"
#include "string.h"

plic_emulator_t *alloc_plic_emulator(void)
{
    plic_emulator_t *plic = kalloc(sizeof(plic_emulator_t));
    if (plic == NULL)
    {
        return NULL;
    }

    memory_set(plic->priority_registers, 0x00,
               sizeof(uint32_t) * SIFIVE_IRQ_NUMBER_MAX);

    return plic;
}

bool plic_emulate_store(virtual_cpu_t *vcpu, uint64_t target, uint64_t value,
                        uint8_t width)
{
    uint64_t offset = target - SIFIVE_PLIC_PRIORITY_BASE;

    // priority registers
    // there are 136 sources(devices).
    if ((SIFIVE_PLIC_PRIORITY_BASE <= target) &&
        (target < (SIFIVE_PLIC_PRIORITY_BASE + 0x220)))
    {
        uint64_t selector = offset / sizeof(uint32_t);
        /*  qemu-virt irqs
         * 1 : virtio-irq
         * 8 : virtio-count?
         * 10: uart0
         * 32: pcie-irq
         */

        if (width != RV64_LOAD_STORE_WIDTH_WORD)
        {
            // XXX is it ok?
            return false;
        }

        vcpu->plic->priority_registers[selector] = value;
        // TODO emulation
        return true;
    }

    // TODO implmeent for other registers.

    return false;
}
