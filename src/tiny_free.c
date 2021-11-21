/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tiny_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matruman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 19:43:44 by matruman          #+#    #+#             */
/*   Updated: 2021/11/21 19:43:47 by matruman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

void	tiny_free(void *ptr, t_tiny_page *page, t_tiny_page *prev)
{
	long	index;

	index = (ptr - (void *)page - sizeof(t_tiny_page)) / TINY_SIZE;
	set_bitmap((unsigned char *) &(page->bitmap), (int)index, 0);
	page->used--;
	if (page->used == 0 && page != g_malloc_data.tiny_malloc_data)
	{
		prev->next = page->next;
		munmap(page, g_malloc_data.pagesize);
	}
}

void	*tiny_realloc(void *ptr, size_t size,
			t_tiny_page *page, t_tiny_page *prev)
{
	void	*res;

	if (size <= 0)
	{
		tiny_free(ptr, page, prev);
		return (NULL);
	}
	else if (size <= TINY_SIZE)
		return (ptr);
	else
	{
		res = ft_malloc(size);
		ft_memcpy(res, ptr, TINY_SIZE);
		tiny_free(ptr, page, prev);
		return (res);
	}
}
