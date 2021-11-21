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
	{
		prev_page->next = page->next;
		g_malloc_data.last_large = page->next;
	}
	else
		g_malloc_data.large_malloc_data = NULL;
	g_malloc_data.last_large = page->next;
	munmap(page, page->size);
}
