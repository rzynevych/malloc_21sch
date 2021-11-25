#include "lib_malloc.h"

# define COUNT 5

struct block_arr
{
	t_block blocks[127];
};

int main()
{
	void	*arr[COUNT];
	int		i;

	t_malloc 		*malloc_data = &g_malloc_data;

	for (i = 0; i < COUNT; i++)
		arr[i] = ft_malloc(5000);

	arr[0] = ft_realloc(arr[0], 10000);

	arr[2] = ft_realloc(arr[2], 10000);

	arr[4] = ft_realloc(arr[4], 10000);

	arr[0] = ft_realloc(arr[0], 5000);

	arr[2] = ft_realloc(arr[2], 5000);

	arr[4] = ft_realloc(arr[4], 5000);

	show_alloc_mem();

	for (i = COUNT; i >= 0; i--)
		ft_free(arr[i]);

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

	struct block_arr *blockArr = (struct block_arr *)(g_malloc_data.small_malloc_data + 1);


	struct block_arr
	{
		t_block blocks[127];
	};
*/