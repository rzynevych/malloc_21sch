/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matruman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 20:36:28 by matruman          #+#    #+#             */
/*   Updated: 2021/11/21 20:36:30 by matruman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

static void	show_tiny_page(t_tiny_page *page, size_t *total, int i, int j)
{
	i = 0;
	print_header("TINY", page);
	while (i < TINY_BITMAP_SIZE)
	{
		j = 0;
		while (j < 8)
		{
			if (page->bitmap[i] & ft_pow2(j))
			{
				print_addr((void *) page + sizeof(t_tiny_page)
					+ (i * 8 + j) * TINY_SIZE);
				ft_putstr(" - ");
				print_addr((void *) page + sizeof(t_tiny_page)
					+ (i * 8 + j + 1) * TINY_SIZE - 1);
				ft_putstr(" : ");
				print_uint64_t(TINY_SIZE);
				ft_putstr(" bytes\n");
				*total += TINY_SIZE;
			}
			j++;
		}
		i++;
	}
}

static size_t	show_tiny(void)
{
	t_tiny_page		*page;
	size_t			total;

	total = 0;
	page = g_malloc_data.tiny_malloc_data;
	while (page)
	{
		show_tiny_page(page, &total, 0, 0);
		page = page->next;
	}
	ft_putstr("Total tiny: ");
	print_uint64_t(total);
	ft_putstr(" bytes\n\n");
	return (total);
}

static size_t	show_small(t_page *page, t_block *block, size_t total)
{
	total = 0;
	page = g_malloc_data.small_user_data;
	while (page)
	{
		print_header("SMALL", page);
		block = page->blocks;
		while (block)
		{
			print_addr(block->ptr);
			ft_putstr(" - ");
			print_addr(block->ptr + block->size - 1);
			ft_putstr(" : ");
			print_uint64_t(block->size);
			ft_putstr(" bytes\n");
			total += block->size;
			block = block->next;
		}
		page = page->next;
	}
	ft_putstr("Total small: ");
	print_uint64_t(total);
	ft_putstr(" bytes\n\n");
	return (total);
}

static size_t	show_large(void)
{
	t_large_page	*page;
	size_t			total;

	total = 0;
	page = g_malloc_data.large_malloc_data;
	if (page == NULL)
		return (0);
	while (page)
	{
		ft_putstr("LARGE : ");
		print_addr(page);
		ft_putstr("\n");
		print_addr((void *) page + sizeof(t_large_page));
		ft_putstr(" - ");
		print_addr((void *) page + page->size - 1);
		ft_putstr(" : ");
		print_uint64_t(page->size - sizeof(t_large_page));
		ft_putstr(" bytes\n");
		total += (page->size - sizeof(t_large_page));
		page = page->next;
	}
	ft_putstr("Total large: ");
	print_uint64_t(total);
	ft_putstr(" bytes\n\n");
	return (total);
}

void	show_alloc_mem(void)
{
	size_t	total;

	total = 0;
	total += show_tiny();
	total += show_small(NULL, NULL, 0);
	total += show_large();
	ft_putstr("Total: ");
	print_uint64_t(total);
	ft_putstr(" bytes\n\n");
}
