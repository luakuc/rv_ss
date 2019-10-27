#include <stdint.h>
#include <stdbool.h>

#include "memory_map.h"
#include "plic.h"
#include "trap.h"
#include "uart.h"
#include "sbi.h"
#include "timer.h"
#include "utils.h"
#include "stdio.h"
#include "memory_manager.h"
#include "virtual_memory.h"
#include "virtio_mmio.h"

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

    while(true)
    {
        count_kthread++;
        __asm__ volatile("wfi");
    }
}

void start_kernel(uint64_t hart_id, uintptr_t device_tree_base)
{
    bool result;
    result = init_memory_manager(memory_map);
    if(!result)
    {
        panic("failed the init_memory_manager");
    }

    result = init_trap(hart_id);
    if(!result)
    {
        panic("failed the init_trap");
    }

    result = init_virtual_memory();
    if(!result)
    {
        panic("failed at init_virtual_memory");
    }

    result = init_plic(&memory_map[VIRT_PLIC]);
    if(!result)
    {
        panic("failed the init_plic.");
    }

    result = init_uart(&memory_map[VIRT_UART0]);
    if(!result)
    {
        panic("failed the init_uart");
    }

    result = init_virtio_mmio(&memory_map[VIRT_VIRTIO]);
    if(!result)
    {
        panic("failed the init_virtio");
    }

    result = init_timer();
    if(!result)
    {
        panic("failed the init_timer");
    }

    void init_test_thread();
    init_test_thread(kthread_0);

    put_string("hello\n");

    enable_interrupt();

    while(true) {
        count_main++;
        __asm__ volatile("wfi");
    }
}
