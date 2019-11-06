#include "vcpu.h"
#include "csr_func.h"
#include "register.h"
#include "memory_manager.h"
#include "virtual_memory.h"

#define HSTATUS_SPRV (0x1 << 0)
#define HSTATUS_SP2V (0x1 << 7)
#define HSTATUS_SP2P (0x1 << 8)
#define HSTATUS_STL (0x1 << 9)
#define HSTATUS_SPV (0x1 << 10)
#define HSTATUS_VTVM    (0x1 << 20)
#define HSTATUS_VTW (0x1 << 21)
#define HSTATUS_VTSR (0x1 << 22)


void vcpu_set_pc(virtual_cpu_t* vcpu, uint64_t pc)
{
    vcpu->guest_context.sepc = pc;
}

void vcpu_set_sp(virtual_cpu_t* vcpu, uint64_t sp)
{
    vcpu->guest_context.sp = sp;
}

bool setup_test_guest(virtual_cpu_t* vcpu, uint64_t guest_func)
{
    page_table_t kernel_page_table = get_kernel_page_table();
    vcpu->vcsr.vsatp = (uint64_t)kernel_page_table;

    uint64_t sp = (uint64_t)kalloc_4k();
    if(!sp)
    {
        return false;
    }

    sp += 0x1000 - 0x10;
    vcpu_set_sp(vcpu, sp);

    vcpu_set_pc(vcpu, (uint64_t)guest_func);
}

void vcpu_store_vcsr(virtual_cpu_t* vcpu)
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
}

void vcpu_load_vcsr(virtual_cpu_t* vcpu)
{
    csr_write_vsstatus(vcpu->vcsr.vsstatus);
    csr_write_vsip(vcpu->vcsr.vsip);
    csr_write_vsie(vcpu->vcsr.vsie);
    csr_write_vstvec(vcpu->vcsr.vstvec);
    csr_write_vsscratch(vcpu->vcsr.vsscratch);
    csr_write_vsepc(vcpu->vcsr.vsepc);
    csr_write_vscause(vcpu->vcsr.vscause);
    csr_write_vstvec(vcpu->vcsr.vstvec);
    csr_write_vsatp(vcpu->vcsr.vsatp);
}

void switch_to_guest(virtual_cpu_t* vcpu);

void run_guest(virtual_cpu_t* vcpu)
{
    vcpu_load_vcsr(vcpu);

    //TODO
    //csr_write_hgatp(vcpu->);
    //hfence.gvma

    switch_to_guest(vcpu);
}

static bool init_vcpu(virtual_cpu_t *vcpu)
{
    vcpu_store_vcsr(vcpu);

    vcpu->guest_context.hstatus = HSTATUS_SPV | HSTATUS_SP2V | HSTATUS_SP2P;
    vcpu->guest_context.sstatus = SSTATUS_SPP | SSTATUS_SIE;

    vcpu->hgatp.

    //TODO
    return true;
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
