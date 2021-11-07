#include "lib_malloc.h"

t_page		*get_page(int size, t_page *source)
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
	return page;
}

t_bool	init_user_page(t_page *prev, int size)
{
	prev->next = default_mmap(SMALL_ALLOC_MULTIPLIER * g_malloc_data.pagesize);
	if (prev->next == NULL)
		return (FALSE);
	return (TRUE);
}

t_sys_page	*init_sys_page(t_sys_page *prev)
{

}

void		init_block(t_sys_page *sys_page, t_page page, t_block *prev)
{

	t_block		*block;

	if (sys_page->blocks_count == 0)
		block = sys_page + sizeof(sys_page);
	block->page = 

}

t_block		*emplace_new_block(t_page *page, t_block *prev_block, int size)
{
	t_sys_page		*sys_page;
	t_sys_page		*prev_page;

	sys_page = g_malloc_data.small_malloc_data;
	while(sys_page->blocks_count == (SMALL_ALLOC_MULTIPLIER * g_malloc_data.pagesize - sizeof(t_page)) / sizeof(t_block))
	{
		prev_page = sys_page;
		sys_page = sys_page->next;
	}
	if (sys_page == NULL)
	{
		if (init_sys_page(prev_page) == FALSE)
			return NULL;
		sys_page = prev_page->next;
	}
}


t_block		*get_block(t_page *page, t_page *prev, int size)
{
	t_block		*block;
	t_block		*new;

	if (page == NULL)
	{
		if (init_user_page(prev, size) == FALSE)
			return (NULL);
		return (prev->next->blocks);
	}
	block = page->blocks;
	while (block->next != NULL &&
		block->next->data - (block->data + block->size) < size)
		block = block->next;
	emplace_new_block(page, block, size);
	
}

void	*small_alloc(size_t sise)
{

}