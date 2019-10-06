#include <stddef.h>

#include "stdio.h"
#include "uart.h"

void put_char(char c)
{
    write_char_by_uart(c);
}

void put_string(const char *string)
{
    for(size_t i = 0; string[i]; ++i)
    {
        put_char(string[i]);
    }
}
