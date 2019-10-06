#pragma once

#include "stdio.h"
#include "sbi.h"

void panic(const char * text)
{
    put_string(text);
    put_char('\n');
    sbi_shutdown();
}
