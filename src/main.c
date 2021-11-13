#include "lib_malloc.h"

int main()
{
    void *p = ft_malloc(256);
    void *p1 = ft_malloc(256);
    printf("%p\n", p);
    printf("%p\n", p1);

    return 0;
}