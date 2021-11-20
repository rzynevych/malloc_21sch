#include "lib_malloc.h"

void		decrease_blocks(t_sys_page *sys_page)
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

void		handle_blocks(t_ptrbox box)
{
	if (box.prev_free && box.block->ptr == box.prev_free->ptr + box.prev_free->size)
	{
		box.prev_free->size += box.block->size;
		if (box.prev_block)
			box.prev_block->next = box.block->next;
		else
			box.page->blocks = box.block->next;
		box.block->ptr = NULL;
		decrease_blocks(syspg_fblk(box.block));
		if (box.next_free && box.block == box.next_free->prev)
		{
			box.prev_free->size += box.next_free->size;
			box.prev_free->next = box.next_free->next;
			box.next_free->ptr= NULL;
			decrease_blocks(syspg_fblk(box.next_free));
		}
		box.freed = box.prev_free;
	}
	else if (box.block == box.next_free->prev)
	{
		box.next_free->ptr = box.block->ptr;
		box.next_free->size += box.block->size;
		if (box.prev_block)
			box.prev_block->next = box.block->next;
		else
			box.page->blocks = box.block->next;
		box.block->ptr = NULL;
		decrease_blocks(syspg_fblk(box.block));
		box.freed = box.next_free;
	} else {
		if (box.prev_block)
			box.prev_block->next = box.block->next;
		else
			box.page->blocks = box.block->next;
		box.block->prev = box.prev_block;
		box.block->next = box.next_free;
		if (box.prev_free)
			box.prev_free->next = box.block;
		else
			box.page->empty_blocks = box.block;
		box.freed = box.block;
	}
	if (box.freed->size > box.page->max_area)
	{
		box.page->max_area = box.freed->size;
		box.page->max_empty = box.freed;
		if (box.page->max_area == SMALL_ALLOC_MULTIPLIER * g_malloc_data.pagesize - sizeof(t_page)
				&& box.page != g_malloc_data.small_user_data)
		{
			box.prev_page->next = box.page->next;
			munmap(box.page,  SMALL_ALLOC_MULTIPLIER * g_malloc_data.pagesize);
		}
	}
}

void		free_block(t_ptrbox box)
{
	box.next_free = box.page->empty_blocks;
	while (box.next_free && box.next_free->ptr < box.block->ptr)
	{
		box.prev_free = box.next_free;
		box.next_free = box.next_free->next;
	}
	handle_blocks(box);
}

void 	small_free(void *ptr, t_page *page, t_page *prev_page)
{
	t_ptrbox box;

	ft_bzero(&box, sizeof(t_ptrbox));
	box.page = page;
	box.prev_page = prev_page;
	box.prev_block = NULL;
	box.block = page->blocks;
	while (box.block->ptr != ptr)
	{
		box.prev_block = box.block;
		box.block = box.block->next;
	}
	free_block(box);
}