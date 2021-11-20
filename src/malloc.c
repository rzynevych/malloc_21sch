#include "lib_malloc.h"

t_malloc g_malloc_data = {};

t_bool   init_malloc()
{
    g_malloc_data.pagesize = getpagesize();
    g_malloc_data.small_malloc_data = init_sys_page();
    if (g_malloc_data.small_malloc_data == NULL)
    {
        g_malloc_data.pagesize = 0;
        return (FALSE);
    }
    g_malloc_data.small_user_data = init_user_page();
    if (g_malloc_data.small_user_data == NULL)
    {
        munmap(g_malloc_data.small_malloc_data, g_malloc_data.pagesize);
        g_malloc_data.pagesize = 0;
        return FALSE;
    }
    return TRUE;
}

void    *ft_malloc(size_t size)
{
    if (g_malloc_data.pagesize == 0)
        if (init_malloc() == FALSE)
            return NULL;
    return (small_alloc((int)size));
}