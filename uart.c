#include "uart.h"

static uintptr_t uart_mmio_base;

// read mode
#define RECEIVE_HOLDING_REGISTER    0b000
#define INTERRUPT_STATUS_REGISTER   0b010
#define LINE_STATUS_REGISTER        0b101
#define MODEM_STATUS_REGISTER       0b110
#define SCRATCHPAD_REGISTER_READ    0b111

// write mode
#define TRANSMIT_HOLDING_REGISTER   0b000
#define LSB_OF_DIVISOR_LATCH        0b000
#define INTERRUPT_ENABLE_REGISTER   0b001
#define MSB_OF_DIVISOR_LATCH        0b001
#define FIFO_CONTROL_REGISTER       0b010
#define LINE_CONTROL_REGISTER       0b011
#define MODEM_CONTROL_REGISTER      0b100
#define SCRATCHPAD_REGISTER_WRITE   0b111

static void register_write(uint8_t offset, uint8_t value)
{
    *(uint8_t *)(uart_mmio_base + offset) = value;
}

static uint8_t register_read(uint8_t offset)
{
    return *(uint8_t*)(uart_mmio_base + offset);
}

bool init_uart(const struct memory_map_entry* uart_entry)
{
    uart_mmio_base = uart_entry->base;

    register_write(INTERRUPT_ENABLE_REGISTER, 0x00);

    register_write(LINE_CONTROL_REGISTER, 0x80);

    register_write(LSB_OF_DIVISOR_LATCH, 0x03);
    register_write(MSB_OF_DIVISOR_LATCH, 0x00);

    register_write(LINE_CONTROL_REGISTER, 0x03);
    register_write(FIFO_CONTROL_REGISTER, 0x07);

    register_write(INTERRUPT_ENABLE_REGISTER, 0x01);

    return true;
}

void write_char_by_uart(char c)
{
    while(register_read(LINE_STATUS_REGISTER) & (1 << 5) == 0);
    register_write(TRANSMIT_HOLDING_REGISTER, c);
}

bool read_char_by_uart(uint8_t* c)
{
    if(register_read(LINE_STATUS_REGISTER) & 0x1)
    {
        *c = register_read(RECEIVE_HOLDING_REGISTER);
        return true;
    }
    else
    {
        return false;
    }
}
