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
		arr[i] = malloc(16);

	arr[0] = realloc(arr[0], 10);

	arr[2] = realloc(arr[2], 100);

	arr[4] = realloc(arr[4], 10000);

	arr[0] = realloc(arr[0], 2);

	arr[2] = realloc(arr[2], 2);

	arr[4] = realloc(arr[4], 2);

	show_alloc_mem();

	for (i = COUNT; i >= 0; i--)
		free(arr[i]);

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