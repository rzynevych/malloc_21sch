/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matruman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 19:43:58 by matruman          #+#    #+#             */
/*   Updated: 2021/11/21 19:44:00 by matruman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_malloc.h"

void			p(char *s)
{
	ft_putstr(s);
	ft_putstr("\n");
}

void	print_header(const char *s, void *addr)
{
	ft_putstr(s);
	ft_putstr(" : ");
	print_addr(addr);
	ft_putstr("\n");
}

t_sys_page	*syspg_fblk(t_block *block)
{
	t_sys_page	*page;
	void		*p;

	p = (void *) block;
	page = (t_sys_page *)(p - (uint64_t) p % g_malloc_data.pagesize);
	return (page);
}

void	*default_mmap(size_t size)
{
	void	*ptr;

	ptr = mmap(0, size, PROT_READ | PROT_WRITE,
			   MAP_ANON | MAP_PRIVATE, -1, 0);
	if (ptr == MAP_FAILED)
		return (NULL);
	return (ptr);
}
