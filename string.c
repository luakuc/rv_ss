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

bool string_compare(const char *str_1, const char *str_2, const size_t size)
{
    if (!str_1 || !str_2)
    {
        return false;
    }

    for (int i = 0; i < size; ++i)
    {
        if (str_1[i] != str_2[i])
        {
            return false;
        }

        // TODO
        // if(str_1[i] == '\0' || str_2[i] == '\0') { }
    }

    return true;
}

size_t string_length(const char *str)
{
    int length = 0;
    for (length = 0; str[length]; ++length)
    {
    }
    return length;
}

static const char hex_map[] = {
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
