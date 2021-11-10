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
	t_page	*page;

	page = default_mmap(SMALL_ALLOC_MULTIPLIER * g_malloc_data.pagesize);
	if (page == NULL)
		return (FALSE);
	page->next = NULL;
	page->type = SMALL_BLOCK_TYPE;
	prev->next = page;
	page->blocks = NULL;
	return (TRUE);
}

t_bool	*init_sys_page(t_sys_page *prev)
{
	t_sys_page	sys_page;
	t_block		*block;

	sys_page = default_mmap(SYS_PAGE_MULTIPLIER * g_malloc_data.pagesize);
	if (sys_page == NULL)
		return (FALSE);
	block = sys_page + sizeof(t_sys_page);
	block->ptr = -1;
	sys_page->blocks_count = 0;
	sys_page->next = NULL;
	prev->next = sys_page;
	return (TRUE);
}

void		set_max_area(t_page *page)
{
	int		max_area;
	t_block	*block;
	t_block *max;

	block = page->empty_blocks;
	max_area = 0;
	while (block) 
	{
		if (block->size < max_area)
		{
			max_area = block->size;
			max= block;
		}
		block = block->next;
	}
	page->max_area = max_area;
	page->max_empty = max;
}

void		find_free_area(t_page *page, t_block *block, int size)
{
	t_block		*empty_block;
	t_block		*prev;

	empty_block = page->empty_blocks;
	prev = NULL;
	while (empty_block != NULL && empty_block->size < size)
	{
		prev = empty_block;
		empty_block = empty_block->next;
	}
	empty_block->size -= size;
	block->ptr = empty_block->ptr;
	empty_block->ptr = (void *)(empty_block->ptr) + size;
	if (empty_block->prev == NULL) 
	{
		if (page->blocks == NULL)
			page->blocks = block;
	}
	else
	{
		if (empty_block->size == 0)
		{
			empty_block->ptr = NULL;
			if (prev != NULL)
				prev->next = empty_block->next;
			else
				page->empty_blocks = empty_block->next;
			if (empty_block->next != NULL)
				empty_block->next->prev = block;
		}
		else
		block->next = empty_block->prev->next;
		empty_block->prev->next = block;
	}
	if (page->max_empty == empty_block)
		set_max_area(page);
}

void		*init_block(t_sys_page *sys_page, t_page *page, int size)
{

	t_block		*block;
	int 		i;
	
	block = (void *)sys_page + sizeof(sys_page);
	i = 0;
	while (block[i].ptr != 0)
		i++;
	block += i;
	block->page = page;
	block->size = size;
	sys_page->blocks_count++;
	find_free_area(page, block, size);
	return (block->ptr);
}

t_block		*init_free_block(t_sys_page *sys_page, t_page *page, int size)
{
	t_block		*block;
	int 		i;
	
	block = (void *)sys_page + sizeof(sys_page);
	i = 0;
	while (block[i].ptr != 0)
		i++;
	block += i;
	block->next = NULL;
	block->page = page;
	block->size = size;
	sys_page->blocks_count++;
	find_free_area(page, block, size);
}

void		*emplace_new_block(t_page *page, int size)
{
	t_sys_page		*sys_page;
	t_sys_page		*prev_page;

	sys_page = g_malloc_data.small_malloc_data;
	while(sys_page->blocks_count == (g_malloc_data.pagesize * SYS_PAGE_MULTIPLIER - sizeof(t_page)) / sizeof(t_block))
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
	return (init_block(sys_page, page, size));
}


void		*allocate_on_page(t_page *page, t_page *prev, int size)
{
	t_block		*block;
	t_block		*new;

	if (page == NULL)
	{
		if (init_user_page(prev, size) == FALSE)
			return (NULL);
	}
	return (emplace_new_block(page, size));
}

void	*small_alloc(size_t sise)
{

}