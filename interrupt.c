#include "interrupt.h"
#include "uart.h"

bool handle_external_interrupt(uint64_t irq)
{
    switch(irq)
    {
        case uart_0:
        {
            //debug
            uint8_t c;
            bool result = read_char_by_uart(&c);
            if(!result)
            {
                //TODO
            }
            write_char_by_uart((char)c);
            break;
        }
        default:
            //unknown irq received
            goto fail;
    }

    return true;

fail:
    return false;
}
