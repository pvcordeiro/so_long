/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:14:46 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/13 22:29:26 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

t_game	*get_game(void)
{
	static t_game	game;

	return (&game);
}
unsigned int	*get_pixel(t_img *data, int x, int y)
{
	return ((unsigned int*)(data->addr + (y * data->line_len + x * (data->bpp / 8))));
}

int	exit_game(void)
{
	mlx_destroy_window(get_game()->mlx, get_game()->win);
	mlx_destroy_display(get_game()->mlx);
	free(get_game()->mlx);
	ft_printf("Game closed\n");
	exit(EXIT_SUCCESS);
}

void	draw_image(t_img *src, t_img *dst, int x, int y)
{
	int	sx;
	int	sy;
	
	sy = 0;
	while(sy < src->height)
	{
		sx = 0;
		while(sx < src->width)
		{
			if (!(sx + x > WINDOW_WIDTH || sy + y > WINDOW_HEIGHT || sx + x < 0 || sy + y < 0) && (*get_pixel(src, sx, sy) != 0xFF000000))
				*get_pixel(dst, x + sx, y + sy) = *get_pixel(src, sx, sy);
			sx++;
		}
		sy++;
	}
}

void	clear_background(void)
{
	int	x;
	int	y;

	y = -1;
	while(++y <= WINDOW_HEIGHT / 40)
	{
		x = -1;
		while(++x <= (WINDOW_WIDTH / 40))
		{
			if ((x + y) % 2 == 0)
				draw_image(&get_game()->floor, &get_game()->canvas, x * 40, y * 40);
			else
				draw_image(&get_game()->floor2, &get_game()->canvas, x * 40, y * 40);
		}
	}
}

int	key_loop(int key, char *action)
{
	if (key == XK_w)
		get_game()->move_up = (*action == 'p');
	if (key == XK_a)
		get_game()->move_left = (*action == 'p');
	if (key == XK_s)
		get_game()->move_down = (*action == 'p');
	if (key == XK_d)
		get_game()->move_right = (*action == 'p');
	if (key == XK_Escape)
		exit_game();
	return (0);
}

t_img	make_sprite(char *path)
{
	t_img	sprite;
	
	if (path)
	{
		sprite.img = mlx_xpm_file_to_image(get_game()->mlx, path, &sprite.width, &sprite.height);
		if (!sprite.img)
			exit_game();
	}
	else
		sprite.img = mlx_new_image(get_game()->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite.x = 0;
	sprite.y = 0;
	sprite.addr = mlx_get_data_addr(sprite.img, &sprite.bpp, &sprite.line_len, &sprite.endian);
	return (sprite);
}
static void init_player(void)
{
    t_player *player;

    player = &get_game()->player;
    player->idle_right[0] = make_sprite("assets/player/idle_right/player_idle_right00.xpm");
    player->idle_right[1] = make_sprite("assets/player/idle_right/player_idle_right01.xpm");
    player->idle_left[0] = make_sprite("assets/player/idle_left/player_idle_left00.xpm");
    player->idle_left[1] = make_sprite("assets/player/idle_left/player_idle_left01.xpm");
    player->move_right[0] = make_sprite("assets/player/move_right/player_move_right00.xpm");
    player->move_right[1] = make_sprite("assets/player/move_right/player_move_right01.xpm");
    player->move_left[0] = make_sprite("assets/player/move_left/player_move_left00.xpm");
    player->move_left[1] = make_sprite("assets/player/move_left/player_move_left01.xpm");
    player->state = IDLE_RIGHT;
    player->current_frame = 0;
    player->anim_counter = 0;
    player->anim_speed = 20;
    player->x = 0;
    player->y = 0;
}
static void update_player_state(void)
{
    t_player *player;

    player = &get_game()->player;
    if (get_game()->move_right)
        player->state = MOVE_RIGHT;
    else if (get_game()->move_left)
        player->state = MOVE_LEFT;
    else if (player->state == MOVE_RIGHT)
        player->state = IDLE_RIGHT;
    else if (player->state == MOVE_LEFT)
        player->state = IDLE_LEFT;
}
static void update_player_animation(void)
{
    t_player *player;

    player = &get_game()->player;
    player->anim_counter++;
    if (player->anim_counter >= player->anim_speed)
    {
        player->anim_counter = 0;
        player->current_frame = !player->current_frame;
    }
}
static void draw_player(void)
{
    t_player *player;
    t_img    *current_sprite;

    player = &get_game()->player;
    if (player->state == IDLE_RIGHT)
        current_sprite = &player->idle_right[player->current_frame];
    else if (player->state == IDLE_LEFT)
        current_sprite = &player->idle_left[player->current_frame];
    else if (player->state == MOVE_RIGHT)
        current_sprite = &player->move_right[player->current_frame];
    else
        current_sprite = &player->move_left[player->current_frame];
    draw_image(current_sprite, &get_game()->canvas, player->x, player->y);
}

int	game_loop(void)
{
	static struct	timeval last_frame = {0, 0};
    struct timeval	current_time;
    long			elapsed_us;

	gettimeofday(&current_time, NULL);
	elapsed_us = (current_time.tv_sec - last_frame.tv_sec) * 1000000 +
                (current_time.tv_usec - last_frame.tv_usec);
	if (elapsed_us < FRAME_DELAY)
    {
        usleep(FRAME_DELAY - elapsed_us);
        return (0);
    }
    last_frame = current_time;
	if (get_game()->move_up)
		get_game()->player.y -= 3;
	if (get_game()->move_down)
		get_game()->player.y += 3;
	if (get_game()->move_left)
		get_game()->player.x -= 3;
	if (get_game()->move_right)
		get_game()->player.x += 3;
	update_player_state();
	update_player_animation();
	clear_background();
	draw_player();
	mlx_put_image_to_window(get_game()->mlx, get_game()->win, get_game()->canvas.img, 0, 0);
	return (0);
}

static void	init_window(void)
{
	get_game()->mlx = mlx_init();
	get_game()->win = mlx_new_window(get_game()->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "so_long");
	if (!get_game()->mlx || !get_game()->win)
		exit_game();
}

static void	init_sprites(void)
{
	get_game()->canvas = make_sprite(NULL);
	get_game()->floor = make_sprite("assets/floor/floor00.xpm");
	get_game()->floor2 = make_sprite("assets/floor/floor01.xpm");
	get_game()->wall = make_sprite("assets/walls/walls00.xpm");
	init_player();
}

static void	setup_hooks(void)
{
	mlx_loop_hook(get_game()->mlx, game_loop, NULL);
	mlx_hook(get_game()->win, KeyPress, KeyPressMask, key_loop, "p");
	mlx_hook(get_game()->win, KeyRelease, KeyReleaseMask, key_loop, "r");
	mlx_hook(get_game()->win, DestroyNotify, KeyPressMask, exit_game, NULL);
}

int	main(void)
{
	init_window();
	init_sprites();
	setup_hooks();
	mlx_loop(get_game()->mlx);
}
