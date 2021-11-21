/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   small_alloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matruman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 19:42:27 by matruman          #+#    #+#             */
/*   Updated: 2021/11/21 19:42:29 by matruman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

t_page	*get_page(int size, t_page *source)
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

t_block	*init_free_block(t_sys_page *sys_page, t_page *page, int size)
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

void	set_max_area(t_page *page)
{
	int			max_area;
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

void	find_free_area(t_page *page, t_block *block, int size)
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
	if (page->blocks == NULL)
		page->blocks = block;
	else
	{
		if (empty_block->prev == NULL)
		{
			block->next = page->blocks;
			page->blocks = block;
		}
		else
		{
			block->next = empty_block->prev->next;
			empty_block->prev->next = block;
		}
	}
	empty_block->prev = block;
	if (empty_block->size == 0)
	{
		empty_block->ptr = NULL;
		if (prev == NULL)
			page->empty_blocks = empty_block->next;
		else
			prev->next = empty_block->next;
		if (empty_block->next != NULL)
			empty_block->next->prev = block;
		decrease_blocks(syspg_fblk(empty_block));
	}	
	if (page->max_empty == empty_block)
		set_max_area(page);
}

void	*init_block(t_sys_page *sys_page, t_page *page, int size)
{
	t_block		*block;
	int			i;

	block = (void *)sys_page + sizeof(t_sys_page);
	i = 0;
	while (block[i].ptr != 0)
		i++;
	block += i;
	block->size = size;
	block->next = NULL;
	sys_page->blocks_count++;
	find_free_area(page, block, size);
	return (block->ptr);
}

void	*emplace_new_block(t_page *page, int size)
{
	t_sys_page		*sys_page;

	sys_page = get_sys_page(g_malloc_data.small_malloc_data);
	if (sys_page == NULL)
		return (NULL);
	return (init_block(sys_page, page, size));
}

void	*small_alloc(int size)
{
	t_page		*page;
	void		*ptr;

	page = get_page(size, g_malloc_data.small_user_data);
	if (page->max_area < size)
	{
		page->next = init_user_page();
		if (page->next == NULL)
			return (NULL);
		page = page->next;
	}
	ptr = emplace_new_block(page, size);
	return (ptr);
}
