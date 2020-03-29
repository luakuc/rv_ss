#include <stdio.h>
#include <stdbool.h>

#include "fdt.h"
#include "fdt_core.h"

int main(int argc, char const* argv[])
{
    char buffer[0x2000];

    FILE* fp = fopen("fdt_dump.mem", "r");
    char c;
    int index = 0;
    while(true)
    {
        c = getc(fp);
        if(c == EOF)
        {
            break;
        }
        buffer[index++] = c;
    }
    fclose(fp);

    bool result = init_fdt((uint64_t)buffer);
    if(!result)
    {
        return -1;
    }

    return 0;
}
