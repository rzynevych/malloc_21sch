#include "lib_malloc.h"

int main()
{
	void	*arr[20];
	int		i;

	for (i = 0; i < 5; i++)
		arr[i] = ft_malloc(256);

	ft_free(arr[1]);

	ft_realloc(arr[0], 512);

	return 0;
}