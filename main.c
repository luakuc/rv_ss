#include <stdbool.h>
#include <stdint.h>

#include "fdt.h"
#include "io_interface.h"
#include "memory_manager.h"
#include "memory_map.h"
#include "plic.h"
#include "sbi.h"
#include "timer.h"
#include "trap.h"
#include "uart.h"
#include "utils.h"
#include "virtio_mmio.h"
#include "virtual_memory.h"
#include "vmm/vcpu.h"
#include "vmm/vmm.h"

extern const struct memory_map_entry memory_map[];

static int count_kthread = 0;
static int count_main = 0;

void kthread_0(void)
{
    write_char_by_uart('t');
    write_char_by_uart('h');
    write_char_by_uart('r');
    write_char_by_uart('e');
    write_char_by_uart('a');
    write_char_by_uart('d');
    write_char_by_uart('\n');

    while (true)
    {
        count_kthread++;
        __asm__ volatile("wfi");
    }
}

void guest_func(void)
{
    static int a = 0;
    while (true)
    {
        a++;
    }
    sbi_shutdown();
}

void start_kernel(uint64_t hart_id, uintptr_t device_tree_base)
{
    bool result;

    result = pre_init_memory_manager();
    if (!result)
    {
        // panic("failed the init_memory_manager");
        return;
    }

    result = init_fdt(device_tree_base);
    if (!result)
    {
        // panic("failed the init_fdt");
        return;
    }

    result = post_init_memory_manager();
    if (!result)
    {
        return;
    }

    result = init_trap(hart_id);
    if (!result)
    {
        panic("failed the init_trap");
    }

    result = init_virtual_memory();
    if (!result)
    {
        panic("failed at init_virtual_memory");
    }

    result = init_plic(&memory_map[VIRT_PLIC]);
    if (!result)
    {
        panic("failed the init_plic.");
    }

    result = init_uart(&memory_map[VIRT_UART0]);
    if (!result)
    {
        panic("failed the init_uart");
    }

    //result = init_virtio_mmio(&memory_map[VIRT_VIRTIO]);
    //if (!result)
    //{
    //    put_string("Failed to initialize virtio devices.\n"
    //               "Any virtio devices are disabled\n");
    //    // panic("failed the init_virtio");
    //}

    result = init_timer();
    if (!result)
    {
        panic("failed the init_timer");
    }

    bool is_vmm_enabled = init_vmm();
    if (!is_vmm_enabled)
    {
        // panic("failed the init_vmm");
        put_string("VMM module is disabled\n");
    }

    // void init_test_thread();
    // init_test_thread(kthread_0);

    put_string("hello\n");
    enable_interrupt();

    if(is_vmm_enabled)
    {
        bool run_test_guest(void);
        result = run_test_guest();
        if (!result)
        {
            panic("error occured in run_test_guest\n");
        }
    }

    //void setup_test_guest(virtual_cpu_t * vcpu, uint64_t guest_func);
    //virtual_cpu_t *vcpu = alloc_vcpu();
    //setup_test_guest(vcpu, (uint64_t)guest_func);
    //run_guest(vcpu);

    // panic("finish");

    while (true)
    {
        count_main++;
        __asm__ volatile("wfi");
    }
}
