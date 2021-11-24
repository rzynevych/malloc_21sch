#include "lib_malloc.h"

# define COUNT 10

int main()
{
	void	*arr[COUNT];
	int		i;

	t_malloc *malloc_data = &g_malloc_data;

	arr[0] = ft_malloc(200);
	arr[1] = ft_malloc(200);
	arr[2] = ft_realloc(arr[0], 100);
	arr[3] = ft_realloc(arr[2], 150);

	show_alloc_mem();

	return 0;
}

/*
	for (i = 0; i < COUNT; i++)
		arr[i] = ft_malloc(512);

	show_alloc_mem();	

	for (i = COUNT; i >= 0; i--)
		ft_free(arr[i]);

	show_alloc_mem();
*/