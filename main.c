#include <stdint.h>
#include <stdbool.h>

#include "memory_map.h"
#include "plic.h"
#include "trap.h"
#include "uart.h"
#include "sbi.h"

void start_kernel(uint64_t hart_id, uintptr_t device_tree_base)
{
    bool result;
    //result = init_memory_manager(memory_map);
    //if(!result)
    //{
    //  die("message");
    //}

    result = init_plic(&memory_map[VIRT_PLIC]);
    if(!result)
    {
        //die("message");
    }

    result = init_trap(hart_id);
    if(!result)
    {
        //die("message");
    }

    result = init_uart(&memory_map[VIRT_UART0]);
    if(!result)
    {
        //die("message");
    }

    //sbi_set_timer(0x800);
    enable_interrupt();

    write_char_by_uart('h');
    write_char_by_uart('e');
    write_char_by_uart('l');
    write_char_by_uart('l');
    write_char_by_uart('o');
    write_char_by_uart('\n');

    while(true) {
        __asm__ volatile("wfi");
    }
}
