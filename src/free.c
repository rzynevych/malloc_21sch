#include "lib_malloc.h"

t_bool	ptr_in_page(t_page *page, void *ptr)
{
	return (t_bool) (ptr >= (void *) page + sizeof(t_page)
			&& ptr < (void *) page + SMALL_ALLOC_MULTIPLIER * g_malloc_data.pagesize);
}

void    ft_free(void *ptr)
{
	t_page *page;
	t_page *prev_page;

	prev_page = NULL;
	page = g_malloc_data.small_user_data;
	while (page && !ptr_in_page(page, ptr))
	{
		prev_page = page;
		page = page->next;
	}
	if (page == NULL)
		return ;
	small_free(ptr, page, prev_page);
}
