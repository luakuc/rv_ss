#include "plic.h"
#include "memory_map.h"
#include "smp.h"

bool init_plic(const struct memory_map_entry *plic_entry)
{
    uintptr_t priority_register = SIFIVE_PLIC_PRIORITY_BASE;
    uintptr_t enable_register = SIFIVE_PLIC_S_ENABLE_BASE;
    // uintptr_t priority_thresholds = SIFIVE_PLIC_PRIORITY_THRESHOLD;
    // uint32_t* claim_complete = (uint32_t*)SIFIVE_PLIC_CLAIM_COMPLETE;

    uint32_t *address = (uint32_t *)(priority_register + 10 * 4);
    // irq of uart0 is 10.
    //*(priority_register + 10) = 1;
    *address = 1;
    // for(int i=0; i<5; ++i)
    //{
    //    *(priority_register + i) = 0;
    //}

    *(uint32_t *)enable_register = (1 << 10);
    // for(int i=0; i<5; ++i)
    //{
    //    *(enable_register + i) = 0;
    //}

    *(uint32_t *)priority_register = 0;

    return true;
}

uint64_t plic_claim(void)
{
    uint64_t cpu_id = get_cpu_id();

    // 64bit ? 32bit?
    uint64_t *claim_address =
        (uint64_t *)SIFIVE_PLIC_S_CLAIM_COMPLETE + cpu_id * 0x2000;

    uint64_t irq = *claim_address;

    return irq;
}

void plic_complete(uint64_t irq)
{
    uint64_t cpu_id = get_cpu_id();

    uint64_t *complete_address =
        (uint64_t *)SIFIVE_PLIC_S_CLAIM_COMPLETE + cpu_id * 0x2000;

    *complete_address = irq;
}
