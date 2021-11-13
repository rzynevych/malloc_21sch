#include "lib_malloc.h"

void	ft_bzero(void *s, size_t n)
{
    size_t	i;

    i = 0;
    while (i < n)
    {
        ((char *)s)[i] = 0;
        i++;
    }
}

void    *default_mmap(size_t size)
{
    return (mmap(0, size, PROT_READ | PROT_WRITE, 
        MAP_ANON | MAP_PRIVATE, -1, 0));
}