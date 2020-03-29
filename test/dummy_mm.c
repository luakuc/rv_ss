#include "memory_manager.h"

#include <stdlib.h>

void *kalloc(const size_t size)
{
    return malloc(size);
}

//void *kalloc_4k(void)
//{
//    return kalloc(0x1000);
//}
//
//void *kalloc_16k(void)
//{
//    return kalloc(0x4000);
//}
