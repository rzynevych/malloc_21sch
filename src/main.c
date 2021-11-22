#include "lib_malloc.h"

int main()
{
	void	*arr[20];
	int		i;

	for (i = 0; i < 30; i++)
		arr[i] = ft_malloc(4000);

	for (i = 0; i < 40; i++)
		ft_free(arr[i]);

	// ft_free(arr[1]);
	// arr[1] = ft_malloc(16);

	ft_realloc(arr[0], 512);

	show_alloc_mem();

	return 0;
}