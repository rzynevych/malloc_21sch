#ifndef LIB_MALLOC_H
# define LIB_MALLOC_H

# include <sys/mman.h>
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <stdio.h>

# define SMALL_BLOCK_TYPE 'S'
# define SMALL_ALLOC_MULTIPLIER 8
# define SYS_PAGE_MULTIPLIER 1

# define TRUE 1
# define FALSE 0

typedef struct s_block	t_block;
typedef char			t_bool;

typedef struct  s_page
{
	struct s_page	*next;
	t_block			*blocks;
	t_block			*empty_blocks;
	t_block			*max_empty;
	int				max_area;
}               	t_page;

typedef struct			s_sys_page
{
	struct s_sys_page	*next;
	int					blocks_count;
}						t_sys_page;

struct  s_block
{
	void			*ptr;
	struct s_block	*next;
	struct s_block	*prev;
    int				size;
};

typedef struct	s_ptrbox
{
	t_block		*block;
	t_block		*prev;
	t_block		*prev_free;
	t_block		*next_free;
	t_block		*freed;
	t_page		*page;
	t_page		*prev_page;
}				t_ptrbox;

typedef struct  s_malloc
{
	int		    pagesize;
	t_sys_page  *small_malloc_data;
	t_page	    *small_user_data;
}               t_malloc;

extern t_malloc		g_malloc_data;

void	    ft_bzero(void *s, size_t n);
void        *default_mmap(size_t size);
t_sys_page  *init_sys_page();
t_page      *init_user_page();
void		*small_alloc(int size);
void        *ft_malloc(size_t size);

#endif