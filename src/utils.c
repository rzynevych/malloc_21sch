#include "lib_malloc.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((char *)s)[i] = 0;
		i++;
	}
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	if (!dst && !src)
		return (dst);
	i = 0;
	while (i < n)
	{
		((char *)dst)[i] = ((char *)src)[i];
		i++;
	}
	return (dst);
}

t_sys_page	*syspg_fblk(t_block *block)
{
	t_sys_page	*page;
	void		*p;

	p = (void *) block;
	page = (t_sys_page *) (p - (uint64_t) p % g_malloc_data.pagesize);
	return (page);
}

void    *default_mmap(size_t size)
{
	return (mmap(0, size, PROT_READ | PROT_WRITE, 
		MAP_ANON | MAP_PRIVATE, -1, 0));
}