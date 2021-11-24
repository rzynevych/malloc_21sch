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
	g_malloc_data.small_malloc_data = init_sys_page();
	if (g_malloc_data.small_malloc_data == NULL)
	{
		g_malloc_data.pagesize = 0;
		return (FALSE);
	}
	g_malloc_data.small_user_data = init_user_page();
	if (g_malloc_data.small_user_data == NULL)
	{
		munmap(g_malloc_data.small_malloc_data, g_malloc_data.pagesize);
		g_malloc_data.pagesize = 0;
		return (FALSE);
	}
	return (TRUE);
}

void	*ft_malloc(size_t size)
{
	size_t	allign;
	size_t	word_lendth;

	word_lendth = sizeof(size_t);
	allign = (word_lendth - size % word_lendth);
	if (size % word_lendth == 0)
		allign = 0;
	size += allign;
	if (g_malloc_data.pagesize == 0)
		if (init_malloc() == FALSE)
			return (NULL);
	if (size <= TINY_SIZE)
		return (tiny_alloc());
	else if (size <= LARGE_START)
		return (small_alloc((size)));
	return (large_alloc(size));
}
