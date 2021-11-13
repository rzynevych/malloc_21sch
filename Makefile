NAME = malloc_test
CC = gcc

SRC = src/main.c \
      src/malloc.c \
      src/small_alloc.c \
      src/utils.c

HEADER_DIR	=	includes/
HEADER		=	includes/ft_malloc.h \
				includes/lib_malloc.h


CFLAGS = -g -I $(HEADER_DIR)

OBJ = $(patsubst %.c,%.o,$(SRC))

%.o: %.c $(HEADER)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all