#include "lib_malloc.h"

void    *default_mmap(size_t size)
{
    return (mmap(0, size, PROT_READ | PROT_WRITE, 
        MAP_ANON | MAP_PRIVATE, -1, 0));
}