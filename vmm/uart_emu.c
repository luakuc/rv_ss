#include "uart_emu.h"
#include "uart_regs.h"
#include "utils.h"
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
    switch(target)
    {
        case LINE_STATUS_REGISTER:
        {
            *value = LSR_RX_DATA_READY | LSR_TX_HOLDING_EMPTY;
            break;
        }
        default:
        {
            not_yet_implemented(true, "vmm, uart: not yet implemented");
            // not yet implemented
            return false;
        }

    }
    //if(width != 0)
    //{
    //    return false;
    //}
    //TODO
    //

    return true;
}

bool uart_emulation_store(virtual_cpu_t *vcpu, uint64_t target, uint64_t value, uint8_t width)
{
    //uint64_t offset = target - 0x10000000;
    if(width != 0)
    {
        return false;
    }
    //TODO

    return true;
}
