#include "lib_malloc.h"

int main()
{
	void	*arr[20];
	int		i;

	for (i = 0; i < 5; i++)
		arr[i] = ft_malloc(256);

	ft_free(arr[0]);
	ft_free(arr[2]);
	ft_free(arr[1]);

	return 0;
}