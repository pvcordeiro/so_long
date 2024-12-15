/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:14:46 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/15 16:51:46 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

t_game	*get_game(void)
{
	static t_game	game;

	return (&game);
}
unsigned int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

static int check_collision(int x1, int y1, int x2, int y2, unsigned int width, unsigned int height)
{
    return (ft_abs(x1 - x2) < width && ft_abs((y1 + COLLISION_Y_OFFSET) - y2) < height);
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

static void draw_text_background(void)
{
    int x;
    int y;
    
    y = WINDOW_HEIGHT - 30;
    while (y < WINDOW_HEIGHT - 15)
    {
        x = 25;
        while (x < 100)
        {
            *get_pixel(&get_game()->canvas, x, y) = 0x00000000;
            x++;
        }
        y++;
    }
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

static void update_collectible(void)
{
    t_collectible *collectible;
    t_player *player;

    collectible = &get_game()->collectible;
    player = &get_game()->player;

    if (!collectible->collected && 
        check_collision(player->x, player->y, 
                       collectible->base.x, collectible->base.y, 20, 20))
    {
        collectible->collected = 1;
        get_game()->collectible_count++;
    }
    if (!collectible->collected)
        update_animation(&collectible->base);
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
	collectible->collected = 0;
	get_game()->collectible_count = 0;
}

static void init_wall(void)
{
    t_wall *wall;

    wall = &get_game()->wall;
    wall->base.sprites[0] = make_sprite("assets/terrain/lava00.xpm");
    wall->base.sprites[1] = make_sprite("assets/terrain/lava01.xpm");
    init_animation(&wall->base, 2, 25);
    wall->base.x = 200;
    wall->base.y = 200;
}

static void init_enemy(void)
{
    t_enemy *enemy;

    enemy = &get_game()->enemy;
    // enemy->idle_right.sprites[0] = make_sprite("assets/enemy/idle_right/enemy_idle_right00.xpm");
    // enemy->idle_right.sprites[1] = make_sprite("assets/enemy/idle_right/enemy_idle_right01.xpm");
    // init_animation(&enemy->idle_right, 2, 20);
    
    // enemy->idle_left.sprites[0] = make_sprite("assets/enemy/idle_left/enemy_idle_left00.xpm");
    // enemy->idle_left.sprites[1] = make_sprite("assets/enemy/idle_left/enemy_idle_left01.xpm");
    // init_animation(&enemy->idle_left, 2, 20);
    
    enemy->move_right.sprites[0] = make_sprite("assets/enemy/move_right/enemy_move_right00.xpm");
    enemy->move_right.sprites[1] = make_sprite("assets/enemy/move_right/enemy_move_right01.xpm");
    init_animation(&enemy->move_right, 2, 20);
    
    enemy->move_left.sprites[0] = make_sprite("assets/enemy/move_left/enemy_move_left00.xpm");
    enemy->move_left.sprites[1] = make_sprite("assets/enemy/move_left/enemy_move_left01.xpm");
    init_animation(&enemy->move_left, 2, 20);
    
	srand(time(NULL));
	enemy->direction = (rand() % 2) * 2 - 1;
	enemy->y_direction = (rand() % 3) - 1;
    enemy->state = (enemy->direction == 1) ? MOVE_RIGHT : MOVE_LEFT;
    enemy->x = 300;
    enemy->y = 300;
    enemy->move_counter = 0;
}

static void update_enemy(void)
{
    t_enemy *enemy;
    t_animation *current_anim;
	int	prev_x;
	int	prev_y;

    enemy = &get_game()->enemy;
	prev_x = enemy->x;
	prev_y = enemy->y;
    enemy->move_counter++;
    
    if (enemy->move_counter >= 100)
    {
        enemy->direction *= -1;
		enemy->y_direction = (rand() % 3) - 1;
        enemy->move_counter = 0;
        enemy->state = (enemy->direction == 1) ? MOVE_RIGHT : MOVE_LEFT;
    }
    
    enemy->x += enemy->direction * 1;
	enemy->y += enemy->y_direction * 1;
	
	if (check_collision(enemy->x, enemy->y, get_game()->wall.base.x, get_game()->wall.base.y, 40, 40))
	{
		enemy->x = prev_x;
		enemy->y = prev_y;
		enemy->direction *= -1;
		enemy->y_direction *= -1;
		enemy->state = (enemy->direction == 1) ? MOVE_RIGHT : MOVE_LEFT;
	}

	if (enemy->y < 0)
	{
		enemy->y = 0;
		enemy->y_direction *= 1;
	}
	if (enemy->y > WINDOW_HEIGHT - 40)
	{
		enemy->y = WINDOW_HEIGHT - 40;
		enemy->y_direction *= 1;
	}

    if (enemy->state == MOVE_RIGHT)
        current_anim = &enemy->move_right;
    else
        current_anim = &enemy->move_left;
    
    update_animation(current_anim);
}

static void draw_enemy(void)
{
    t_enemy *enemy;
    t_animation *current_anim;

    enemy = &get_game()->enemy;
    if (enemy->state == MOVE_RIGHT)
        current_anim = &enemy->move_right;
    else
        current_anim = &enemy->move_left;

    current_anim->x = enemy->x;
    current_anim->y = enemy->y;
    draw_animation(current_anim, &get_game()->canvas);
}

void	draw_floor(void)
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

static void init_exit(void)
{
    t_exit *exit;

    exit = &get_game()->exit;
    exit->sprite = make_sprite("assets/exit.xpm");
    exit->x = 400;
    exit->y = 400;
}

static void draw_exit(void)
{
    t_exit *exit;

    exit = &get_game()->exit;
    draw_image(&exit->sprite, &get_game()->canvas, exit->x, exit->y);
}

static void init_health(void)
{
    t_health *health;

    health = &get_game()->health;
    health->health1 = make_sprite("assets/health/health1.xpm");
    health->health2 = make_sprite("assets/health/health2.xpm");
    health->health3 = make_sprite("assets/health/health3.xpm");
}

static void draw_health(void)
{
    t_health *health;
    t_img *current_sprite;

    health = &get_game()->health;
    if (get_game()->player.lives == 3)
        current_sprite = &health->health3;
    else if (get_game()->player.lives == 2)
        current_sprite = &health->health2;
    else if (get_game()->player.lives == 1)
        current_sprite = &health->health1;
    else
        return;
    draw_image(current_sprite, &get_game()->canvas, WINDOW_WIDTH - 100, 20);
}

static void init_player(void)
{
    t_player *player;

    player = &get_game()->player;
	get_game()->move_count = 0;
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
	player->last_direction = MOVE_RIGHT;
    player->x = 0;
    player->y = 0;
	player->lives = 3;
	player->invincibility_frames = 0;
	player->is_visible = 1;
}


static void update_player_position(void)
{
    int prev_x;
    int prev_y;
    t_player *player;
    t_wall *wall;

    player = &get_game()->player;
    wall = &get_game()->wall;
    prev_x = player->x;
    prev_y = player->y;

	if (get_game()->move_up || get_game()->move_down || get_game()->move_left || get_game()->move_right)
	{
		if (get_game()->move_up)
			player->y -= 2;
		if (get_game()->move_down)
			player->y += 2;
		if (get_game()->move_left)
			player->x -= 2;
		if (get_game()->move_right)
			player->x += 2;
	}

    if (check_collision(player->x, player->y, wall->base.x, wall->base.y, 30, 25))
    {
        player->x = prev_x;
        player->y = prev_y;
    }
	else if (prev_x != player->x || prev_y != player->y)
		get_game()->move_count++;
}

static void update_player(void)
{
    t_player *player;
    t_animation *current_anim;

    player = &get_game()->player;
    if (get_game()->move_right)
	{
        player->state = MOVE_RIGHT;
		player->last_direction = MOVE_RIGHT;
	}
    else if (get_game()->move_left)
	{
        player->state = MOVE_LEFT;
		player->last_direction = MOVE_LEFT;
	}
	else if (get_game()->move_up || get_game()->move_down)
	{
		if (player->last_direction == MOVE_RIGHT)
			player->state = MOVE_RIGHT;
		else
			player->state = MOVE_LEFT;
	}
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
	if (player->invincibility_frames > 0)
	{
		player->is_visible = !player->is_visible;
		if (!player->is_visible)
			return ;
	}
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

static void	fps_cap(void)
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
		return ;
	}
	last_frame = current_time;
}

int	game_loop(void)
{
	char	*print_move;

	if (get_game()->player.invincibility_frames > 0)
        get_game()->player.invincibility_frames--;

    if (check_collision(get_game()->player.x, get_game()->player.y, 
        get_game()->enemy.x, get_game()->enemy.y, 10, 20) && 
        get_game()->player.invincibility_frames == 0)
    {
        get_game()->player.lives--;
        get_game()->player.invincibility_frames = 60;
        if (get_game()->player.lives <= 0)
        {
            ft_printf("Game Over\n");
            exit_game();
        }
    }
	fps_cap();
	draw_floor();
	update_collectible();
	update_animation(&get_game()->collectible.base);
	if(!get_game()->collectible.collected)
		draw_animation(&get_game()->collectible.base, &get_game()->canvas);
	update_enemy();
	draw_enemy();
	draw_exit();
	update_animation(&get_game()->wall.base);
	draw_animation(&get_game()->wall.base, &get_game()->canvas);
	update_player_position();
	update_player();
	draw_player();
	draw_health();
	if (get_game()->collectible.collected)
    {
        if (check_collision(get_game()->player.x, get_game()->player.y, 
                       get_game()->exit.x, get_game()->exit.y, 20, 20))
        {
            ft_printf("Victory!\n");
            exit_game();
        }
    }
	draw_text_background();
	mlx_put_image_to_window(get_game()->mlx, get_game()->win, get_game()->canvas.img, 0, 0);
	print_move = ft_itoa(get_game()->move_count);
	mlx_string_put(get_game()->mlx, get_game()->win, 30, WINDOW_HEIGHT - 18, 0x00FFFFFF, "MOVES:");
	mlx_string_put(get_game()->mlx, get_game()->win, 70, WINDOW_HEIGHT - 18, 0x00FFFFFF, print_move);
	ft_printf("Moves: %s\n", print_move);
	free(print_move);
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
	get_game()->floor = make_sprite("assets/terrain/floor00.xpm");
	get_game()->floor2 = make_sprite("assets/terrain/floor01.xpm");
	init_collectible();
	init_wall();
	init_player();
	init_enemy();
	init_exit();
	init_health();
}

static void	setup_hooks(void)
{
	mlx_loop_hook(get_game()->mlx, game_loop, NULL);
	mlx_hook(get_game()->win, KeyPress, KeyPressMask, key_loop, "p");
	mlx_hook(get_game()->win, KeyRelease, KeyReleaseMask, key_loop, "r");
	mlx_hook(get_game()->win, DestroyNotify, KeyPressMask, exit_game, NULL);
}

// int main(int argc, char **argv)
// {
//     t_map *map_info;

//     if (argc != 2)
//         return (ft_printf("Error\nUsage: ./so_long [map.ber]\n"));
//     map_info = parse_map(argv[1]);
//     if (!map_info)
//         return (ft_printf("Error\nInvalid map\n"));
//     init_window();
//     init_sprites();
//     setup_hooks();
//     mlx_loop(get_game()->mlx);
//     return (0);
// }

int	main(void)
{
	init_window();
	init_sprites();
	setup_hooks();
	mlx_loop(get_game()->mlx);
}
