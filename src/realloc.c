#include "lib_malloc.h"

t_bool 	small_search(void *ptr, void **res, int size)
{
	t_page	*page;
	t_page	*prev_page;

	page = g_malloc_data.small_user_data;
	prev_page = NULL;
	while (page && !ptr_in_page(page, ptr))
	{
		prev_page = page;
		page = page->next;
	}
	if (page != NULL)
	{
		*res = small_realloc(ptr, size, page, prev_page);
		return (TRUE);
	}
	return (FALSE);
}

void 	*realloc(void *ptr, size_t size)
{
	void 	*res;

	if (small_search(ptr, &res, size))
		return (res);
	return (NULL);
}