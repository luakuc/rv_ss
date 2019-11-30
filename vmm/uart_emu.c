#include "uart_emu.h"
#include "memory_manager.h"

uart_emulator_t *alloc_uart_emulator(void)
{
    uart_emulator_t *uart = kalloc(sizeof(uart_emulator_t));
    if(uart == NULL)
    {
        return NULL;
    }

    return uart;
}

bool uart_emulation_load(virtual_cpu_t* vcpu, uint64_t target, uint64_t* value, uint8_t width)
{
    //if(width != 0)
    //{
    //    return false;
    //}
    //TODO

    return true;
}

bool uart_emulation_store(virtual_cpu_t *vcpu, uint64_t target, uint64_t value, uint8_t width)
{
    //uint64_t offset = target - 0x10000000;
    if(width != 0)
    {
        return false;
    }

    return true;
}
