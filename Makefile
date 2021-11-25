ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME =		libft_malloc_$(HOSTTYPE).so
SL_NAME =	libft_malloc.so
CC = gcc

SRC = src/libft.c \
      src/malloc.c \
      src/realloc.c \
      src/free.c \
      src/small_free.c \
      src/small_alloc.c \
      src/small_alloc_common.c \
      src/small_common.c \
      src/small_realloc.c \
      src/tiny_alloc.c \
      src/tiny_free.c \
      src/large.c \
      src/show_alloc_mem.c \
      src/print.c \
      src/utils.c

HEADER_DIR	=	includes/
HEADER	=	includes/lib_malloc.h

CFLAGS = -Wall -Wextra -Werror -I $(HEADER_DIR)

OBJ = $(SRC:.c=.o)

%.o: %.c $(HEADER)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -shared -fPIC -o $(NAME)
	ln -fs ${NAME} ${SL_NAME}

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	rm -f $(SL_NAME)

re: fclean all