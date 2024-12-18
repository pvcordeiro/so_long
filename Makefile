NAME = so_long
CC = cc
CFLAGS = -Wall -Wextra -Werror -I includes
MLX_FLAGS = -Lmlx_linux -lmlx_Linux -Imlx_linux -lXext -lX11 -lm -lz -O3

SRCS_DIR = srcs/
INCLUDES_DIR = includes/
SRCS = $(SRCS_DIR)main.c $(SRCS_DIR)ft_printf.c $(SRCS_DIR)get_next_line.c $(SRCS_DIR)libft.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(MLX_FLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(MLX_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

test: re
	./$(NAME) maps/test.ber