#include "lib_malloc.h"

void		handle_blocks(t_ptrbox box)
{
	if (box.prev_free && box.block->ptr == box.prev_free + box.prev_free->size)
	{
		box.prev_free->size += block->size;
		if (box.prev_block)
			box.prev_block->next = box.block->next;
		else
			box.page->blocks = box.block->next;
		box.block->ptr = NULL;
		if (box.next_free && box.block == box.next_free->prev)
		{
			box.prev_free->size += box.next_free->size;
			box.prev_free->next = box.next_free->next;
			box.next_free->ptr= NULL;
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
		if (box.page->max_area == SMALL_ALLOC_MULTIPLIER * g_malloc_data.pagesize - sizeof(t_page))
		{
			box.prev_page->next = box.page->next;
			munmap(box.page,  SMALL_ALLOC_MULTIPLIER * g_malloc_data.pagesize);
		}
	}
}

void		free_block(t_block *block, t_block *prev, t_page *page)
{
	t_ptrbox	box;

	box.block = block;
	box.page = page;
	box.next_free = page->empty_blocks;
	if (box.next_free == NULL)
		return NULL;
	while (box.next_free && box.next_free->ptr < block->ptr)
	{
		box.prev_free = box.next_free;
		box.next_free = box.next_free->next;
	}
	handle_blocks(box);
}

void 	*small_free(void *ptr, t_page *page)
{
	t_block *block;
	t_block	*prev;

	prev = NULL;
	block = page->blocks;
	while (block->ptr != ptr)
	{
		prev = block;
		block = block->next;
	}
	free_block(block, prev page);
}