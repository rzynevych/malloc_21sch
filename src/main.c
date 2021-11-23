#include "lib_malloc.h"

int main()
{
	void	*arr[1000];
	int		i;

	t_malloc *malloc_data = &g_malloc_data;

	for (i = 0; i < 20; i++)
		arr[i] = ft_malloc(8000);

	show_alloc_mem();	

	for (i = 0; i < 40; i++)
		ft_free(arr[i]);

	show_alloc_mem();

	// show_alloc_mem();

	return 0;
}