#include "lib_malloc.h"

void 	tiny_free(void *ptr, t_tiny_page *page, t_tiny_page *prev)
{
	long index;

	index = (ptr - (void *)page) / TINY_SIZE;
	set_bitmap((unsigned char *) &(page->bitmap), (int)index, 0);
	page->used--;
	if (page->used == 0 && page != g_malloc_data.tiny_malloc_data)
	{
		prev->next = page->next;
		munmap(page, g_malloc_data.pagesize);
	}
}

void	*tiny_realloc(void *ptr, size_t size, t_tiny_page *page, t_tiny_page *prev)
{
	void	*res;

	if (size <= 0)
	{
		tiny_free(ptr, page, prev);
		return (NULL);
	}
	else if (size <= TINY_SIZE)
		return (ptr);
	else
	{
		res = ft_malloc(size);
		ft_memcpy(res, ptr, TINY_SIZE);
		tiny_free(ptr, page, prev);
		return (res);
	}
}