/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:14:46 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/13 23:53:33 by paude-so         ###   ########.fr       */
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
int	exit_error(void)
{
	mlx_destroy_window(get_game()->mlx, get_game()->win);
	mlx_destroy_display(get_game()->mlx);
	free(get_game()->mlx);
	ft_printf("Error\n");
	exit(EXIT_FAILURE);
}
t_img	make_sprite(char *path)
{
	t_img	sprite;
	
	if (path)
	{
		sprite.img = mlx_xpm_file_to_image(get_game()->mlx, path, &sprite.width, &sprite.height);
		if (!sprite.img)
			exit_error();
	}
	else
		sprite.img = mlx_new_image(get_game()->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite.x = 0;
	sprite.y = 0;
	sprite.addr = mlx_get_data_addr(sprite.img, &sprite.bpp, &sprite.line_len, &sprite.endian);
	return (sprite);
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

void init_animation(t_animation *anim, int frame_count, int speed)
{
    anim->current_frame = 0;
    anim->anim_counter = 0;
    anim->anim_speed = speed;
    anim->frame_count = frame_count;
}

void update_animation(t_animation *anim)
{
    anim->anim_counter++;
    if (anim->anim_counter >= anim->anim_speed)
    {
        anim->anim_counter = 0;
        anim->current_frame = (anim->current_frame + 1) % anim->frame_count;
    }
}

void draw_animation(t_animation *anim, t_img *canvas)
{
    draw_image(&anim->sprites[anim->current_frame], canvas, anim->x, anim->y);
}

static void init_collectible(void)
{
    t_collectible *collectible;

    collectible = &get_game()->collectible;
    collectible->base.sprites[0] = make_sprite("assets/collect/collect00.xpm");
    collectible->base.sprites[1] = make_sprite("assets/collect/collect01.xpm");
    collectible->base.sprites[2] = make_sprite("assets/collect/collect02.xpm");
    collectible->base.sprites[3] = make_sprite("assets/collect/collect03.xpm");
    init_animation(&collectible->base, 4, 15);
    collectible->base.x = 100;
    collectible->base.y = 100;
}


static void init_wall(void)
{
    t_wall *wall;

    wall = &get_game()->wall;
    wall->base.sprites[0] = make_sprite("assets/wall/wall00.xpm");
    wall->base.sprites[1] = make_sprite("assets/wall/wall01.xpm");
    init_animation(&wall->base, 2, 25);
    wall->base.x = 200;
    wall->base.y = 200;
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

static void init_player(void)
{
    t_player *player;

    player = &get_game()->player;
    player->idle_right.sprites[0] = make_sprite("assets/player/idle_right/player_idle_right00.xpm");
	player->idle_right.sprites[1] = make_sprite("assets/player/idle_right/player_idle_right01.xpm");
	init_animation(&player->idle_right, 2, 20);
	player->idle_left.sprites[0] = make_sprite("assets/player/idle_left/player_idle_left00.xpm");
	player->idle_left.sprites[1] = make_sprite("assets/player/idle_left/player_idle_left01.xpm");
	init_animation(&player->idle_left, 2, 20);
	player->move_right.sprites[0] = make_sprite("assets/player/move_right/player_move_right00.xpm");
	player->move_right.sprites[1] = make_sprite("assets/player/move_right/player_move_right01.xpm");
	init_animation(&player->move_right, 2, 10);
	player->move_left.sprites[0] = make_sprite("assets/player/move_left/player_move_left00.xpm");
	player->move_left.sprites[1] = make_sprite("assets/player/move_left/player_move_left01.xpm");
	init_animation(&player->move_left, 2, 10);
    player->state = IDLE_RIGHT;
    player->x = 0;
    player->y = 0;
}
static void update_player(void)
{
    t_player *player;
    t_animation *current_anim;

    player = &get_game()->player;
    if (get_game()->move_right)
        player->state = MOVE_RIGHT;
    else if (get_game()->move_left)
        player->state = MOVE_LEFT;
    else if (player->state == MOVE_RIGHT)
        player->state = IDLE_RIGHT;
    else if (player->state == MOVE_LEFT)
        player->state = IDLE_LEFT;

    if (player->state == IDLE_RIGHT)
        current_anim = &player->idle_right;
    else if (player->state == IDLE_LEFT)
        current_anim = &player->idle_left;
    else if (player->state == MOVE_RIGHT)
        current_anim = &player->move_right;
    else
        current_anim = &player->move_left;
    
    update_animation(current_anim);
}
static void draw_player(void)
{
    t_player *player;
    t_animation *current_anim;

    player = &get_game()->player;
    if (player->state == IDLE_RIGHT)
        current_anim = &player->idle_right;
    else if (player->state == IDLE_LEFT)
        current_anim = &player->idle_left;
    else if (player->state == MOVE_RIGHT)
        current_anim = &player->move_right;
    else
        current_anim = &player->move_left;

    current_anim->x = player->x;
    current_anim->y = player->y;
    draw_animation(current_anim, &get_game()->canvas);
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
	update_player();
	update_animation(&get_game()->collectible.base);
	update_animation(&get_game()->wall.base);
	clear_background();
	draw_player();
	draw_animation(&get_game()->collectible.base, &get_game()->canvas);
	draw_animation(&get_game()->wall.base, &get_game()->canvas);
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
	init_player();
	init_collectible();
	init_wall();
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
