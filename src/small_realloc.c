/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   small_realloc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matruman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 19:43:06 by matruman          #+#    #+#             */
/*   Updated: 2021/11/21 19:43:08 by matruman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

static void	handle_increase(t_ptrbox box, size_t size)
{
	if (box.block->size + box.next_free->size == size)
	{
		box.block->size = size;
		if (box.prev_free)
			box.prev_free->next = box.next_free->next;
		else
			box.page->empty_blocks = box.next_free->next;
		box.next_free->ptr = NULL;
		decrease_blocks(syspg_fblk(box.next_free));
	}
	else
	{
		box.next_free->size = box.block->size + box.next_free->size - size;
		box.next_free->ptr = box.block->ptr + size;
		box.block->size = size;
	}
	if (box.next_free == box.page->max_empty)
		set_max_area(box.page);
}

static void	*increase_size(t_ptrbox box, size_t size)
{
	void	*ptr;

	box.next_free = box.page->empty_blocks;
	while (box.next_free && box.next_free->ptr < box.block->ptr)
	{
		box.prev_free = box.next_free;
		box.next_free = box.next_free->next;
	}
	if (box.next_free && box.next_free->prev == box.block
		&& box.block->size + box.next_free->size >= size)
	{
		handle_increase(box, size);
		return (box.block->ptr);
	}
	else
	{
		ptr = malloc(size);
		ft_memcpy(ptr, box.block->ptr, box.block->size);
		small_free(box.block->ptr, box.page, box.prev_page);
		return (ptr);
	}
}

static void	*handle_free_block(t_ptrbox *box, size_t size)
{
	t_sys_page	*sys_page;

	sys_page = get_sys_page(g_malloc_data.small_malloc_data);
	if (sys_page == NULL)
		return (NULL);
	box->freed = init_free_block(sys_page, box->page, box->block->size - size);
	box->freed->ptr = box->block->ptr + size;
	if (box->prev_free)
		box->prev_free->next = box->freed;
	else
		box->page->empty_blocks = box->freed;
	box->freed->next = box->next_free;
	box->freed->prev = box->block;
	box->block->size = size;
	if (box->freed->size > box->page->max_area)
	{
		box->page->max_area = box->freed->size;
		box->page->max_empty = box->freed;
	}
	return (box->block->ptr);
}

static void	*decrease_size(t_ptrbox box, size_t size)
{
	box.next_free = box.page->empty_blocks;
	while (box.next_free && box.next_free->ptr < box.block->ptr)
	{
		box.prev_free = box.next_free;
		box.next_free = box.next_free->next;
	}
	if (box.next_free && box.next_free->prev == box.block)
	{
		box.next_free->size = box.block->size + box.next_free->size - size;
		box.next_free->ptr = box.block->ptr + size;
		box.block->size = size;
		if (box.next_free->size > box.page->max_area)
		{
			box.page->max_area = box.next_free->size;
			box.page->max_empty = box.next_free;
		}
		return (box.block->ptr);
	}
	else
		return (handle_free_block(&box, size));
}

void	*small_realloc(void	*ptr, size_t size, t_page *page, t_page *prev_page)
{
	t_ptrbox	box;

	ft_bzero(&box, sizeof(t_ptrbox));
	box.page = page;
	box.prev_page = prev_page;
	if (ptr == NULL)
		return (malloc(size));
	box.block = page->blocks;
	while (box.block && box.block->ptr != ptr)
		box.block = box.block->next;
	if (box.block == NULL)
		return (NULL);
	if (box.block->size < size)
		return (increase_size(box, size));
	else if (box.block->size > size && size > 0)
		return (decrease_size(box, size));
	else if (size == 0)
	{
		small_free(ptr, page, prev_page);
		return (NULL);
	}
	else
		return (ptr);
}
