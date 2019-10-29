#include "vmm.h"
#include "vcpu.h"

virtual_cpu_t *vcpu;

bool init_vmm(void)
{
    vcpu = alloc_vcpu();
    if(!vcpu)
    {
        return false;
    }


    return true;
}
