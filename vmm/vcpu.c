#include "vcpu.h"
#include "csr_func.h"
#include "memory_manager.h"

static bool init_vcpu(virtual_cpu_t *vcpu)
{
    vcpu->vcsr.vsstatus = csr_read_vsstatus();
    vcpu->vcsr.vsip = csr_read_vsip();
    vcpu->vcsr.vsie = csr_read_vsie();
    vcpu->vcsr.vstvec = csr_read_vstvec();
    vcpu->vcsr.vsscratch = csr_read_vsscratch();
    vcpu->vcsr.vsepc = csr_read_vsepc();
    vcpu->vcsr.vscause = csr_read_vscause();
    vcpu->vcsr.vstvec = csr_read_vstvec();
    vcpu->vcsr.vsatp = csr_read_vsatp();

    //TODO
    return false;
}

virtual_cpu_t *alloc_vcpu(void)
{
    virtual_cpu_t *vcpu = kalloc(sizeof(virtual_cpu_t));
    if (!vcpu)
    {
        return NULL;
    }

    bool result = init_vcpu(vcpu);
    if(!result)
    {
        return false;
    }

    return vcpu;
}
