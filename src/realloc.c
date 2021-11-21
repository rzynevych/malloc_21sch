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

static t_bool	small_search(void *ptr, void **res, int size)
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
	if (page != NULL)
	{
		*res = small_realloc(ptr, size, page, prev_page);
		return (TRUE);
	}
	return (FALSE);
}

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
	if (page)
	{
		*res = tiny_realloc(ptr, size, page, prev);
		return (TRUE);
	}
	return (FALSE);
}

void	*ft_realloc(void *ptr, size_t size)
{
	void	*res;

	if (small_search(ptr, &res, (int) size))
		return (res);
	else if (tiny_search(ptr, &res, (int) size))
		return (res);
	return (NULL);
}
