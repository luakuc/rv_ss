#include "uart_emu.h"
#include "uart_regs.h"
#include "utils.h"
#include "memory_manager.h"
#include "io_interface.h"
#include "string.h"

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

    switch(target)
    {
        case TRANSMIT_HOLDING_REGISTER:
        {
            char c = (uint8_t)value;
            put_char(c);
            break;
        }
        case INTERRUPT_ENABLE_REGISTER:
        case LINE_CONTROL_REGISTER:
        //case LSB_OF_DIVISOR_LATCH:
        //case MSB_OF_DIVISOR_LATCH:
        case FIFO_CONTROL_REGISTER:
        {
            put_string("vmm,uart: not implemented yet.\n");
            break;
        }
        default:
        {
            char target_str[8];
            char value_str[10];
            int_to_str(target, target_str);
            int_to_str(value, value_str);
            put_string("vmm,uart: [store] not yet implemented\n");
            put_string("vmm,uart: target: 0x");
            put_string(target_str);
            put_string("\n");
            put_string("vmm,uart: value: 0x");
            put_string(value_str);
            put_string("\n");
            return false;
        }
    }
    //TODO

    return true;
}
