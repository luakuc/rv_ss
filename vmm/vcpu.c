#include "vcpu.h"
#include "memory_manager.h"

static bool init_vcpu(virtual_cpu_t* vcpu)
{

}

virtual_cpu_t* alloc_vcpu(void)
{
    virtual_cpu_t* vcpu = kalloc(sizeof(virtual_cpu_t));
    if(!vcpu)
    {
        return NULL;
    }

    return vcpu;
}
