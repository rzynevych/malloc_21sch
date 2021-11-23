#include "lib_malloc.h"

t_page	*get_page(size_t size, t_page *source)
{
	t_page		*page;
	t_page		*prev;

	page = source;
	while (page != NULL && page->max_area < size)
	{
		prev = page;
		page = page->next;
	}
	if (page == NULL)
		return (prev);
	return (page);
}

void	set_max_area(t_page *page)
{
	size_t		max_area;
	t_block		*block;
	t_block		*max;

	block = page->empty_blocks;
	max_area = block->size;
	max = block;
	while (block)
	{
		if (block->size < max_area)
		{
			max_area = block->size;
			max = block;
		}
		block = block->next;
	}
	page->max_area = max_area;
	page->max_empty = max;
}

void	decrease_blocks(t_sys_page *sys_page)
{
	t_sys_page	*prev_page;

	sys_page->blocks_count--;
	prev_page = g_malloc_data.small_malloc_data;
	if (sys_page->blocks_count == 0 && sys_page != prev_page)
	{
		while (prev_page->next != sys_page)
			prev_page = prev_page->next;
		prev_page->next = sys_page->next;
		munmap(sys_page, g_malloc_data.pagesize);
	}
}