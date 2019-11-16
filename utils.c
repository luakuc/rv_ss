#include "utils.h"

#include "io_interface.h"
#include "sbi.h"

void panic(const char *text)
{
    put_string(text);
    put_char('\n');
    sbi_shutdown();
}
