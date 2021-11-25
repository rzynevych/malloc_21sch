/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matruman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 19:42:14 by matruman          #+#    #+#             */
/*   Updated: 2021/11/21 19:42:17 by matruman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

static t_bool	tiny_search(void *ptr, void **res, int size)
{
	t_tiny_page		*page;
	t_tiny_page		*prev;

	prev = NULL;
	page = g_malloc_data.tiny_malloc_data;
	while (page && page != (ptr - (uint64_t) ptr % g_malloc_data.pagesize))
	{
		prev = page;
		page = page->next;
	}
	if (page == NULL)
		return (FALSE);
	*res = tiny_realloc(ptr, size, page, prev);
	return (TRUE);
}

static t_bool	small_search(void *ptr, void **res, size_t size)
{
	t_page	*page;
	t_page	*prev_page;

	page = g_malloc_data.small_user_data;
	prev_page = NULL;
	while (page && !ptr_in_page(page, ptr))
	{
		prev_page = page;
		page = page->next;
	}
	if (page == NULL)
		return (FALSE);
	*res = small_realloc(ptr, size, page, prev_page);
	return (TRUE);
}

static t_bool	large_search(void *ptr, void **res, size_t size)
{
	t_large_page	*page;
	t_large_page	*prev_page;
	
	prev_page = NULL;
	page = g_malloc_data.large_malloc_data;
	while (page && ptr - sizeof(t_large_page) != (void *)page)
	{
		prev_page = page;
		page = page->next;
	}
	if (!page)
		return (FALSE);
	*res = large_realloc(ptr, size, page, prev_page);
	return (TRUE);
}

void	*ft_realloc(void *ptr, size_t size)
{
	void	*res;
	size_t	align;

	align = (WORD_LENGTH - size % WORD_LENGTH);
	if (size % WORD_LENGTH == 0)
		align = 0;
	size += align;
	res = NULL;
	if ((size_t)(ptr - sizeof(t_large_page)) % g_malloc_data.pagesize == 0)
		if (large_search(ptr, &res, size))
			return (res);
	if (small_search(ptr, &res, (int) size))
		return (res);
	tiny_search(ptr, &res, (int) size);
	return (res);
}
