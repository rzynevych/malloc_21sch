#ifndef LIB_MALLOC_H
# define LIB_MALLOC_H

# include <sys/mman.h>
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <stdio.h>

# define TINY_SIZE 32
# define TINY_BITMAP_SIZE 16
# define SMALL_ALLOC_MULTIPLIER 8
# define LARGE_START 0x1000


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

typedef struct	s_tiny_page
{
	struct s_tiny_page	*next;
	int 				used;
	unsigned char 		bitmap[TINY_BITMAP_SIZE];
}				t_tiny_page;

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
	t_block		*prev_block;
	t_block		*prev_free;
	t_block		*next_free;
	t_block		*freed;
	t_page		*page;
	t_page		*prev_page;
}				t_ptrbox;

typedef struct  s_malloc
{
	int		    pagesize;
	t_tiny_page *tiny_malloc_data;
	t_sys_page  *small_malloc_data;
	t_page	    *small_user_data;

}               t_malloc;

extern t_malloc		g_malloc_data;

void	    ft_bzero(void *s, size_t n);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void        *default_mmap(size_t size);
t_sys_page	*syspg_fblk(t_block *block);
t_sys_page  *init_sys_page();
t_page      *init_user_page();
void		*small_alloc(int size);
t_sys_page	*get_sys_page(t_sys_page *source);
t_block		*init_free_block(t_sys_page *sys_page, t_page *page, int size);
void        *ft_malloc(size_t size);
void 		*ft_realloc(void *ptr, size_t size);
t_bool		ptr_in_page(t_page *page, void *ptr);
void 		small_free(void *ptr, t_page *page, t_page *prev_page);
void		*small_realloc(void	*ptr, int size, t_page *page, t_page *prev_page);
void		decrease_blocks(t_sys_page *sys_page);
void 		*tiny_alloc();
void		*tiny_realloc(void *ptr, size_t size, t_tiny_page *page, t_tiny_page *prev);
void 		tiny_free(void *ptr, t_tiny_page *page, t_tiny_page *prev);
t_tiny_page	*init_tiny_page();
void		set_bitmap(unsigned char *bitmap, int index, int val);
void    	ft_free(void *ptr);

#endif