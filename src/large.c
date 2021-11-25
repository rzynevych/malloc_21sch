/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   large.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matruman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 19:51:10 by matruman          #+#    #+#             */
/*   Updated: 2021/11/21 19:51:13 by matruman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

void	*large_alloc(size_t size)
{
	t_large_page	*page;
	size_t			mmap_size;

	mmap_size = size + sizeof(t_large_page);
	if (mmap_size % g_malloc_data.pagesize != 0)
		mmap_size = (mmap_size / g_malloc_data.pagesize + 1)
			* g_malloc_data.pagesize;
	page = default_mmap(mmap_size);
	if (page == NULL)
		return (NULL);
	page->size = mmap_size;
	page->next = NULL;
	if (g_malloc_data.large_malloc_data == NULL)
	{
		g_malloc_data.large_malloc_data = page;
		g_malloc_data.last_large = page;
	}
	else
	{
		g_malloc_data.last_large->next = page;
		g_malloc_data.last_large = page;
	}
	return ((void *)page + sizeof(t_large_page));
}

void	large_free(t_large_page *page, t_large_page *prev_page)
{
	if (prev_page)
		prev_page->next = page->next;
	else
		g_malloc_data.large_malloc_data = page->next;
	if (page->next == NULL)
		g_malloc_data.last_large = prev_page;
	munmap(page, page->size);
}

size_t	get_copy_size(size_t mmap_size, t_large_page *page)
{
	size_t			copy_size;

	if (mmap_size > page->size)
		copy_size = page->size - sizeof(t_large_page);
	else
		copy_size = mmap_size - sizeof(t_large_page);
	return (copy_size);
}

void	*large_realloc(void *ptr, size_t size, t_large_page *page,
			t_large_page *prev_page)
{
	void 			*res;
	size_t			mmap_size;
	size_t			copy_size;

	mmap_size = size + sizeof(t_large_page);
	if (mmap_size % g_malloc_data.pagesize != 0)
		mmap_size = (mmap_size / g_malloc_data.pagesize + 1)
			* g_malloc_data.pagesize;
	if (mmap_size == page->size)
		return (ptr);	
	res = ft_malloc(size);
	copy_size = get_copy_size(mmap_size, page);
	ft_memcpy(res, ptr, copy_size);
	large_free(page, prev_page);
	return (res);
}