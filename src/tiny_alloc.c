/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tiny_alloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matruman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 19:43:31 by matruman          #+#    #+#             */
/*   Updated: 2021/11/21 19:43:33 by matruman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

void	set_bitmap(unsigned char *bitmap, int index, int val)
{
	unsigned char	c;
	unsigned char	mask;
	int				i;

	c = bitmap[index / 8];
	mask = 1;
	i = 0;
	while (i < index % 8)
	{
		mask *= 2;
		i++;
	}
	if (val == 0)
	{
		mask = ~mask;
		bitmap[index / 8] = c & mask;
	}
	else
		bitmap[index / 8] = c | mask;
}

int	get_index(unsigned char c)
{
	int		i;

	i = 0;
	while (c % 2 != 0)
	{
		c /= 2;
		i++;
	}
	return (i);
}

t_tiny_page	*init_tiny_page(void)
{
	t_tiny_page	*page;

	page = default_mmap(g_malloc_data.pagesize);
	if (page == NULL)
		return (NULL);
	page->next = NULL;
	page->used = 0;
	ft_bzero(&(page->bitmap), TINY_BITMAP_SIZE);
	return (page);
}

t_tiny_page	*get_tiny_page(void)
{
	t_tiny_page		*page;
	t_tiny_page		*prev;

	page = g_malloc_data.tiny_malloc_data;
	while (page && page->used == g_malloc_data.pagesize
		- sizeof(t_tiny_page) / TINY_SIZE)
	{
		prev = page;
		page = page->next;
	}
	if (page != NULL)
		return (page);
	prev->next = init_tiny_page();
	return (prev->next);
}

void	*tiny_alloc(void)
{
	t_tiny_page		*page;
	int				index;
	int				i;

	page = get_tiny_page();
	if (page == NULL)
		return (NULL);
	i = 0;
	while (page->bitmap[i] == 255)
		++i;
	index = i * 8 + get_index(page->bitmap[i]);
	page->used++;
	set_bitmap((unsigned char *) &(page->bitmap), index, 1);
	return ((void *) page + sizeof(t_tiny_page) + TINY_SIZE * index);
}
