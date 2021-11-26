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

static void	handle_blocks(t_page *page, t_block *block,
			t_block *empty_block)
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
	empty_block->prev = block;
}

static void	handle_zero(t_page *page,
			t_block *empty_block, t_block *prev_empty)
{
	if (empty_block->size == 0)
	{
		empty_block->ptr = NULL;
		if (prev_empty == NULL)
			page->empty_blocks = empty_block->next;
		else
			prev_empty->next = empty_block->next;
		decrease_blocks(syspg_fblk(empty_block));
	}	
}

static void	find_free_area(t_page *page, t_block *block, size_t size)
{
	t_block		*empty_block;
	t_block		*prev_empty;

	empty_block = page->empty_blocks;
	prev_empty = NULL;
	while (empty_block != NULL && empty_block->size < size)
	{
		prev_empty = empty_block;
		empty_block = empty_block->next;
	}
	empty_block->size -= size;
	block->ptr = empty_block->ptr;
	empty_block->ptr = empty_block->ptr + size;
	handle_blocks(page, block, empty_block);
	handle_zero(page, empty_block, prev_empty);
	if (page->max_empty == empty_block)
		set_max_area(page);
}

static void	*init_block(t_sys_page *sys_page, t_page *page, size_t size)
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

void	*small_alloc(size_t size)
{
	t_sys_page	*sys_page;
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
	sys_page = get_sys_page(g_malloc_data.small_malloc_data);
	if (sys_page == NULL)
		return (NULL);
	ptr = init_block(sys_page, page, size);
	return (ptr);
}
