/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matruman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 19:41:38 by matruman          #+#    #+#             */
/*   Updated: 2021/11/21 19:41:51 by matruman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

t_malloc	g_malloc_data = {};

t_bool	init_malloc(void)
{
	g_malloc_data.pagesize = getpagesize();
	g_malloc_data.tiny_malloc_data = init_tiny_page();
	if (g_malloc_data.tiny_malloc_data == NULL)
	{
		g_malloc_data.pagesize = 0;
		return (FALSE);
	}
	g_malloc_data.small_malloc_data = init_sys_page();
	if (g_malloc_data.small_malloc_data == NULL)
	{
		munmap(g_malloc_data.tiny_malloc_data, g_malloc_data.pagesize);
		g_malloc_data.pagesize = 0;
		return (FALSE);
	}
	g_malloc_data.small_user_data = init_user_page();
	if (g_malloc_data.small_user_data == NULL)
	{
		munmap(g_malloc_data.tiny_malloc_data, g_malloc_data.pagesize);
		munmap(g_malloc_data.small_malloc_data, g_malloc_data.pagesize);
		g_malloc_data.pagesize = 0;
		return (FALSE);
	}
	return (TRUE);
}

void	*malloc(size_t size)
{
//	p("malloc start");
//	print_uint64_t(size);
//	p(": malloc size");

	if (size == 0)
		size += WORD_LENGTH;
	if (size % WORD_LENGTH != 0)
		size = (size / WORD_LENGTH + 1) * WORD_LENGTH;
	if (g_malloc_data.pagesize == 0)
		if (init_malloc() == FALSE)
			return (NULL);
	if (size <= TINY_SIZE)
		return (tiny_alloc());
	if (size <= LARGE_START)
		return (small_alloc((size)));
	return (large_alloc(size));
}
