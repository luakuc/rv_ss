#include "string.h"

// TODO return value
void memory_set(void *destination, uint8_t byte, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        *((uint8_t *)destination + i) = byte;
    }
}

// TODO return value
void memory_copy(void *destination, void *source, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        *((uint8_t *)destination + i) = *((uint8_t *)source + i);
    }
}

const char hex_map[] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
};

void int_to_str(uint64_t n, char *buffer) //, int radix)
{
    size_t length = 0;
    size_t acc = n;
    while (acc)
    {
        acc /= 0x10;
        length++;
    }

    if (length == 0)
    {
        length = 1;
    }

    for (size_t i = 0; i < length; ++i)
    {
        size_t tmp = n % 0x10;
        n /= 0x10;
        buffer[length - i - 1] = hex_map[tmp];
    }
    buffer[length] = '\0';
}
