#include "string.h"

void *memory_set(void *destination, uint8_t byte, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        *((uint8_t *)destination + i) = byte;
    }
}
