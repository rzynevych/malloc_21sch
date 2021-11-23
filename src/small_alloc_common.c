#include "lib_malloc.h"

t_sys_page	*get_sys_page(t_sys_page *source)
{
	t_sys_page		*page;
	t_sys_page		*prev;

	page = source;
	while (page != NULL && page->blocks_count
		== (g_malloc_data.pagesize - sizeof(t_sys_page)) / sizeof(t_block))
	{
		prev = page;
		page = page->next;
	}
	if (page != NULL)
		return (page);
	prev->next = init_sys_page();
	return (prev->next);
}

t_block	*init_free_block(t_sys_page *sys_page, t_page *page, size_t size)
{
	t_block		*block;
	int			i;

	block = (void *)sys_page + sizeof(t_sys_page);
	i = 0;
	while (block[i].ptr != 0)
		i++;
	block += i;
	block->ptr = (void *)page + sizeof(t_page);
	block->next = NULL;
	block->prev = NULL;
	block->size = size;
	sys_page->blocks_count++;
	return (block);
}

t_page	*init_user_page(void)
{
	t_page		*page;
	t_sys_page	*sys_page;

	sys_page = get_sys_page(g_malloc_data.small_malloc_data);
	page = (t_page *) default_mmap(
			SMALL_ALLOC_MULTIPLIER * g_malloc_data.pagesize);
	if (sys_page == NULL || page == NULL)
		return (NULL);
	page->next = NULL;
	page->blocks = NULL;
	page->max_area = SMALL_ALLOC_MULTIPLIER * g_malloc_data.pagesize
		- sizeof (t_page);
	page->empty_blocks = init_free_block(sys_page, page,
			SMALL_ALLOC_MULTIPLIER * g_malloc_data.pagesize - sizeof(t_page));
	page->max_empty = page->empty_blocks;
	return (page);
}

t_sys_page	*init_sys_page(void)
{
	t_sys_page	*sys_page;

	sys_page = (t_sys_page *) default_mmap(g_malloc_data.pagesize);
	if (sys_page == NULL)
		return (NULL);
	ft_bzero(sys_page, g_malloc_data.pagesize);
	sys_page->blocks_count = 0;
	sys_page->next = NULL;
	return (sys_page);
}