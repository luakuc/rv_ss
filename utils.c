#include "utils.h"

#include "io_interface.h"
#include "sbi.h"

void panic(const char *text)
{
    put_string(text);
    put_char('\n');
    sbi_shutdown();
}

void not_yet_implemented(bool is_critical, const char* message)
{
    if(is_critical)
    {
        panic(message);
    }

    put_string(message);
}
