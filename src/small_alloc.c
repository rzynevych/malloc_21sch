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
	sys_page->closest_free = 1;
	sys_page->next = NULL;
	prev->next = sys_page;
	return (TRUE);
}

void		set_max_area(t_page *page)
{
	int		max_area;
	int		tmp;
	t_block	*block;
	t_block *max_after;

	block = page->blocks;
	max_area = 0;
	while (block->next) 
	{
		
		tmp = block->next->ptr - block->ptr - block->size;
		if (tmp > max_area)
		{
			max_area = tmp;
			max_after = block;
		}
		block = block->next;
	}
	tmp = SMALL_ALLOC_MULTIPLIER * g_malloc_data.pagesize + (void *)page - block->ptr - block->size;
	if (tmp > max_area) 
	{
		max_area = tmp;
		max_after = block;
	}
	page->max_area = max_area;
	page->max_after = max_after;
}

void		find_free_area(t_page *page, t_block *block, int size)
{
	t_block		*curr_block;

	curr_block = page->blocks;
	if (curr_block == NULL)
	{
		page->blocks = block;
		page->max_after = block;
		page->max_area = SMALL_ALLOC_MULTIPLIER * g_malloc_data.pagesize - sizeof(t_page) - size;
		block->next = NULL;
		return (page + sizeof(t_page));
	}
	while (curr_block->next != NULL && (curr_block->next->ptr - curr_block->ptr - curr_block->size) < size)
		curr_block = curr_block->next;
	block->next = curr_block->next;
	block->ptr = curr_block->ptr + curr_block->size;
	curr_block->next = block;
	if (page->max_after == curr_block)
		set_max_area(page);
}

void		*init_block(t_sys_page *sys_page, t_page *page, int size)
{

	t_block		*block;
	int 		i;
	
	block = (void *)sys_page + sizeof(sys_page) + sizeof(t_block);
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
	init_block(sys_page, page, size);
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
	emplace_new_block(page, size);
}

void	*small_alloc(size_t sise)
{

}