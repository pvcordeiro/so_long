NAME = so_long
CC = cc
CFLAGS = -Wall -Wextra -Werror -I includes
MLX_FLAGS = -Lmlx_linux -lmlx_Linux -Imlx_linux -lXext -lX11 -lm -lz -O3
SRCS_DIR = srcs/
CORE_DIR = $(SRCS_DIR)core/
INIT_DIR = $(SRCS_DIR)init/
UPDATE_DIR = $(SRCS_DIR)update/
RENDER_DIR = $(SRCS_DIR)render/
COLLISION_DIR = $(SRCS_DIR)collision/
UTILS_DIR = $(SRCS_DIR)utils/
HANDLE_DIR = $(SRCS_DIR)handle/
CORE_SRCS = $(CORE_DIR)main.c \
            $(CORE_DIR)game_loop.c \
            $(CORE_DIR)game_state.c \
			$(CORE_DIR)cleanup_sprite.c
INIT_SRCS = $(INIT_DIR)init_game.c \
            $(INIT_DIR)init_map.c \
            $(INIT_DIR)init_player.c \
            $(INIT_DIR)init_entities.c \
			$(INIT_DIR)init_enemy.c \
			$(INIT_DIR)init_collectibles.c
UPDATE_SRCS = $(UPDATE_DIR)update_player.c \
              $(UPDATE_DIR)update_enemy.c \
              $(UPDATE_DIR)update_entities.c \
			  $(UPDATE_DIR)update_game_state.c
RENDER_SRCS = $(RENDER_DIR)draw_ui.c \
              $(RENDER_DIR)draw_entities.c \
              $(RENDER_DIR)draw_frame.c
COLLISION_SRCS = $(COLLISION_DIR)collision.c \
				 $(COLLISION_DIR)attack_collision.c
UTILS_SRCS = $(UTILS_DIR)sprite_utils.c \
             $(UTILS_DIR)ft_printf.c \
             $(UTILS_DIR)get_next_line.c \
             $(UTILS_DIR)libft.c
HANDLE_SRCS = $(HANDLE_DIR)handle_movement.c \
			  $(HANDLE_DIR)handle_misc.c
SRCS = $(CORE_SRCS) $(INIT_SRCS) $(UPDATE_SRCS) $(RENDER_SRCS) $(COLLISION_SRCS) $(UTILS_SRCS) $(HANDLE_SRCS)
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