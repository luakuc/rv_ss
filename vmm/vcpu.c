#include "vcpu.h"
#include "csr_func.h"
#include "register.h"
#include "memory_manager.h"

#define HSTATUS_SPRV (0x1 << 0)
#define HSTATUS_SP2V (0x1 << 7)
#define HSTATUS_SP2P (0x1 << 8)
#define HSTATUS_STL (0x1 << 9)
#define HSTATUS_SPV (0x1 << 10)
#define HSTATUS_VTVM    (0x1 << 20)
#define HSTATUS_VTW (0x1 << 21)
#define HSTATUS_VTSR (0x1 << 22)

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

    vcpu->guest_context.hstatus = HSTATUS_SPV;
    vcpu->guest_context.sstatus = SSTATUS_SPP | SSTATUS_SIE;

    //TODO
    return true;
}

void vcpu_set_pc(virtual_cpu_t* vcpu, uint64_t pc)
{
    vcpu->guest_context.sepc = pc;
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
        return NULL;
    }

    return vcpu;
}

void switch_to_guest(virtual_cpu_t* vcpu);

void run_guest(virtual_cpu_t* vcpu)
{
    switch_to_guest(vcpu);
}
