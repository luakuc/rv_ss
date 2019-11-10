#include "vcpu.h"
#include "csr_func.h"
#include "guest_address_translation.h"
#include "memory_manager.h"
#include "register.h"
#include "string.h"
#include "virtual_memory.h"
#include "trap.h"

#define HSTATUS_SPRV (0x1 << 0)
#define HSTATUS_SP2V (0x1 << 7)
#define HSTATUS_SP2P (0x1 << 8)
#define HSTATUS_STL (0x1 << 9)
#define HSTATUS_SPV (0x1 << 10)
#define HSTATUS_VTVM (0x1 << 20)
#define HSTATUS_VTW (0x1 << 21)
#define HSTATUS_VTSR (0x1 << 22)

void vcpu_set_pc(virtual_cpu_t *vcpu, uint64_t pc)
{
    vcpu->guest_context.sepc = pc;
}

void vcpu_set_sp(virtual_cpu_t *vcpu, uint64_t sp)
{
    vcpu->guest_context.sp = sp;
}

bool setup_test_guest(virtual_cpu_t *vcpu, uint64_t guest_func)
{
    page_table_t kernel_page_table = get_kernel_page_table();

    satp_t satp;

    uint64_t ppn = ((uint64_t)kernel_page_table) >> 12;
    satp.ppn = ppn;
    satp.mode = SATP_MODE_SV39;
    satp.asid = 0;

    //vcpu->vcsr.vsatp = satp.value;
    vcpu->vcsr.vsatp = 0;
    vcpu->vcsr.vsstatus = 0x8000000000006000;

    uint64_t sp = (uint64_t)kalloc_4k();
    if (!sp)
    {
        return false;
    }

    bool result = guest_memory_map(
        vcpu->gp_hp_page_table, 0x80000000, 0x80000000, 0x800000,
        PTE_FLAG_USER | PTE_FLAG_READ | PTE_FLAG_WRITE | PTE_FLAG_EXEC);
    if (!result)
    {
        return false;
    }

    //bool result = guest_memory_map(vcpu->gp_hp_page_table, sp, sp, 0x1000,
    //                          PTE_FLAG_READ | PTE_FLAG_WRITE);
    //if (!result)
    //{
    //    return false;
    //}

    sp += 0x1000 - 0x10;
    vcpu_set_sp(vcpu, sp);

    vcpu_set_pc(vcpu, (uint64_t)guest_func);
    //result = guest_memory_map(vcpu->gp_hp_page_table, (uint64_t)guest_func,
    //                          (uint64_t)guest_func, 0x1000,
    //                          PTE_FLAG_READ | PTE_FLAG_WRITE | PTE_FLAG_EXEC);
    //if (!result)
    //{
    //    return false;
    //}

    return true;
}

void vcpu_store_vcsr(virtual_cpu_t *vcpu)
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

void vcpu_load_vcsr(virtual_cpu_t *vcpu)
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

    //hfence_bvma();
}

void switch_to_guest(virtual_cpu_t *vcpu);

void run_guest(virtual_cpu_t *vcpu)
{
    vcpu_load_vcsr(vcpu);

    update_hgatp(vcpu->gp_hp_page_table);

    hfence_gvma();

    switch_to_guest(vcpu);
}

static bool init_vcpu(virtual_cpu_t *vcpu)
{
    vcpu_store_vcsr(vcpu);

    vcpu->guest_context.hstatus = HSTATUS_SPV | HSTATUS_SP2V | HSTATUS_SP2P;
    vcpu->guest_context.sstatus = SSTATUS_SPP | SSTATUS_SIE;

    vcpu->gp_hp_page_table = (page_table_t)kalloc_16k();
    // currently, in kernel mode, this OS uses direct map memory between
    // physical and virtual.
    vcpu->gp_hp_page_table_phy = vcpu->gp_hp_page_table;
    memory_set(vcpu->gp_hp_page_table, 0x00, 0x4000);

    // TODO
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
    if (!result)
    {
        return NULL;
    }

    return vcpu;
}
