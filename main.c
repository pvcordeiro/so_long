/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:14:46 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/16 17:57:02 by paude-so         ###   ########.fr       */
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

static int	check_collision(int x1, int y1, int x2, int y2, unsigned int width,
		unsigned int height)
{
	return (ft_abs(x1 - x2) < width && ft_abs((y1 + COLLISION_Y_OFFSET)
			- y2) < height);
}

static void check_attack_collision(void)
{
    t_player *player;
    t_enemy *enemy;

    player = &get_game()->player;
    enemy = &get_game()->enemy;

    if (!enemy->is_dead && player->is_attacking && enemy->invincibility_frames == 0)
    {
        if ((player->state == ATTACK_RIGHT && enemy->x > player->x) || 
            (player->state == ATTACK_LEFT && enemy->x < player->x))
        {
            if (check_collision(player->x, player->y, enemy->x, enemy->y, 
                              ATTACK_RANGE, ENEMY_COLLISION_HEIGHT))
            {
                enemy->lives--;
                enemy->invincibility_frames = INVINCIBILITY_DURATION / 2;
                if (player->x < enemy->x)
                {
                    enemy->direction = 1;
                    enemy->state = MOVE_RIGHT;
                }
                else
                {
                    enemy->direction = -1;
                    enemy->state = MOVE_LEFT;
                }
                // enemy->x += enemy->direction * 40;
                if (enemy->lives <= 0)
                    enemy->is_dead = true;
            }
        }
    }
}

unsigned int	*get_pixel(t_img *data, int x, int y)
{
	return ((unsigned int *)(data->addr + (y * data->line_len + x * (data->bpp
					/ 8))));
}

static void	check_bounds(t_player *player)
{
	if (player->x < 0)
		player->x = 0;
	if (player->y < 0)
		player->y = 0;
	if (player->x > get_game()->window_width - SPRITE_SIZE)
		player->x = get_game()->window_width - SPRITE_SIZE;
	if (player->y > get_game()->window_height - SPRITE_SIZE)
		player->y = get_game()->window_height - SPRITE_SIZE;
}

static void	cleanup_sprites(void)
{
	t_game	*game;
	int		i;

	game = get_game();
	mlx_destroy_image(game->mlx, game->canvas.img);
	mlx_destroy_image(game->mlx, game->floor.img);
	mlx_destroy_image(game->mlx, game->floor2.img);
	free(game->wall.x_positions);
	free(game->wall.y_positions);
	free(game->collectible.x_positions);
	free(game->collectible.y_positions);
	free(game->collectible.collected);
	i = -1;
	while (++i < 4)
		mlx_destroy_image(game->mlx, game->collectible.base.sprites[i].img);
	i = -1;
	while (++i < 2)
	{
		mlx_destroy_image(game->mlx, game->wall.base.sprites[i].img);
        mlx_destroy_image(game->mlx, game->player.idle_right.sprites[i].img);
        mlx_destroy_image(game->mlx, game->player.idle_left.sprites[i].img);
        mlx_destroy_image(game->mlx, game->player.move_right.sprites[i].img);
        mlx_destroy_image(game->mlx, game->player.move_left.sprites[i].img);
        mlx_destroy_image(game->mlx, game->player.attack_right.sprites[i].img);
        mlx_destroy_image(game->mlx, game->player.attack_left.sprites[i].img);
        mlx_destroy_image(game->mlx, game->enemy.move_right.sprites[i].img);
        mlx_destroy_image(game->mlx, game->enemy.move_left.sprites[i].img);
    }
	mlx_destroy_image(game->mlx, game->exit.sprite.img);
    mlx_destroy_image(game->mlx, game->health.health1.img);
    mlx_destroy_image(game->mlx, game->health.health2.img);
    mlx_destroy_image(game->mlx, game->health.health3.img);
}

int	exit_game(void)
{
	cleanup_sprites();
	mlx_destroy_window(get_game()->mlx, get_game()->win);
	mlx_destroy_display(get_game()->mlx);
	free(get_game()->mlx);
	ft_printf("Game closed\n");
	exit(EXIT_SUCCESS);
}
int	exit_error(void)
{
	cleanup_sprites();
	mlx_destroy_window(get_game()->mlx, get_game()->win);
	mlx_destroy_display(get_game()->mlx);
	free(get_game()->mlx);
	ft_printf("Error\n");
	exit(EXIT_FAILURE);
}

static void	draw_text_background(void)
{
	int	x;
	int	y;

	y = get_game()->window_height - 30;
	while (y < get_game()->window_height - 15)
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
		sprite.img = mlx_xpm_file_to_image(get_game()->mlx, path, &sprite.width,
				&sprite.height);
		if (!sprite.img)
			exit_error();
	}
	else
		sprite.img = mlx_new_image(get_game()->mlx, get_game()->window_width,
				get_game()->window_height);
	sprite.x = 0;
	sprite.y = 0;
	sprite.addr = mlx_get_data_addr(sprite.img, &sprite.bpp, &sprite.line_len,
			&sprite.endian);
	return (sprite);
}

void	draw_image(t_img *src, t_img *dst, int x, int y)
{
	int	sx;
	int	sy;

	sy = 0;
	while (sy < src->height)
	{
		sx = 0;
		while (sx < src->width)
		{
			if (!(sx + x > get_game()->window_width || sy + y > get_game()->window_height || sx + x < 0
					|| sy + y < 0) && (*get_pixel(src, sx, sy) != 0xFF000000))
				*get_pixel(dst, x + sx, y + sy) = *get_pixel(src, sx, sy);
			sx++;
		}
		sy++;
	}
}

void	init_animation(t_animation *anim, int frame_count, int speed)
{
	anim->current_frame = 0;
	anim->anim_counter = 0;
	anim->anim_speed = speed;
	anim->frame_count = frame_count;
}

void	update_animation(t_animation *anim)
{
	anim->anim_counter++;
	if (anim->anim_counter >= anim->anim_speed)
	{
		anim->anim_counter = 0;
		anim->current_frame = (anim->current_frame + 1) % anim->frame_count;
	}
}

void	draw_animation(t_animation *anim, t_img *canvas)
{
	draw_image(&anim->sprites[anim->current_frame], canvas, anim->x, anim->y);
}

static void	update_collectible(void)
{
	t_collectible	*collectible;
	t_player		*player;
	int				i;

	collectible = &get_game()->collectible;
	player = &get_game()->player;
	i = -1;
	while (++i < collectible->count)
	{
		if (!collectible->collected[i] && check_collision(player->x, player->y,
            collectible->x_positions[i], collectible->y_positions[i],
            COLLECTIBLE_SIZE, COLLECTIBLE_SIZE))
		{
			collectible->collected[i] = 1;
			get_game()->collectible_count++;
		}
	}
	update_animation(&collectible->base);
}

static void	init_collectible(void)
{
	t_collectible	*collectible;
	t_map			*map;
	int				i;
	int				j;
	int				count;

	collectible = &get_game()->collectible;
	map = &get_game()->map;
	count = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == 'C')
				count++;
		}
	}
	collectible->count = count;
	collectible->x_positions = malloc(sizeof(int) * count);
	collectible->y_positions = malloc(sizeof(int) * count);
	collectible->collected = malloc(sizeof(int) * count);
	count = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == 'C')
			{
				collectible->x_positions[count] = j * SPRITE_SIZE;
				collectible->y_positions[count] = i * SPRITE_SIZE;
				collectible->collected[count] = 0;
				count++;
			}
		}
	}
	collectible->base.sprites[0] = make_sprite("assets/collect/collect00.xpm");
	collectible->base.sprites[1] = make_sprite("assets/collect/collect01.xpm");
	collectible->base.sprites[2] = make_sprite("assets/collect/collect02.xpm");
	collectible->base.sprites[3] = make_sprite("assets/collect/collect03.xpm");
	init_animation(&collectible->base, 4, COLLECTIBLE_ANIMATION_SPEED);
	get_game()->collectible_count = 0;
}

static void	init_wall(void)
{
	t_wall	*wall;
	int	i;
	int	j;
	int	count;
	t_map *map;

	wall = &get_game()->wall;
	map = &get_game()->map;
	count = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == '1')
				count++;
		}
	}
	wall->count = count;
	wall->x_positions = malloc(sizeof(int) * count);
	wall->y_positions = malloc(sizeof(int) * count);
	count = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == '1')
			{
				wall->x_positions[count] = j * SPRITE_SIZE;
				wall->y_positions[count] = i * SPRITE_SIZE;
				count++;
			}
		}
	}
	wall->base.sprites[0] = make_sprite("assets/terrain/lava00.xpm");
	wall->base.sprites[1] = make_sprite("assets/terrain/lava01.xpm");
	init_animation(&wall->base, FRAME_COUNT, WALL_ANIMATION_SPEED);
}

static int check_wall_collisions(int x, int y)
{
	t_wall	*wall;
	int		i;

	wall = &get_game()->wall;
	i = -1;
	while (++i < wall->count)
	{
		if (check_collision(x, y, wall->x_positions[i], wall->y_positions[i],
				WALL_COLLISION_WIDTH, WALL_COLLISION_HEIGHT))
			return (1);
	}
	return (0);
}

static void	init_enemy(void)
{
	t_enemy	*enemy;

	enemy = &get_game()->enemy;
	enemy->move_right.sprites[0] = make_sprite("assets/enemy/move_right/enemy_move_right00.xpm");
	enemy->move_right.sprites[1] = make_sprite("assets/enemy/move_right/enemy_move_right01.xpm");
	init_animation(&enemy->move_right, FRAME_COUNT, ENEMY_ANIMATION_SPEED);
	enemy->move_left.sprites[0] = make_sprite("assets/enemy/move_left/enemy_move_left00.xpm");
	enemy->move_left.sprites[1] = make_sprite("assets/enemy/move_left/enemy_move_left01.xpm");
	init_animation(&enemy->move_left, FRAME_COUNT, ENEMY_ANIMATION_SPEED);
	srand(time(NULL));
	enemy->direction = (rand() % 2) * 2 - 1;
	enemy->y_direction = (rand() % 3) - 1;
	if (enemy->direction == 1)
		enemy->state = MOVE_RIGHT;
	else
		enemy->state = MOVE_LEFT;
	enemy->x = 150;
	enemy->y = 100;
	enemy->move_counter = 0;
	enemy->lives = 3;
	enemy->is_dead = false;
	enemy->invincibility_frames = 0;
	enemy->is_visible = true;
}

static void	update_enemy(void)
{
	t_enemy		*enemy;
	t_animation	*current_anim;
	int			new_x;
	int			new_y;

	enemy = &get_game()->enemy;
	if (enemy->is_dead)
		return ;
	if (enemy->invincibility_frames > 0)
		enemy->invincibility_frames--;
	enemy->move_counter++;
	if (enemy->move_counter >= ENEMY_MOVE_THRESHOLD)
	{
		if (rand() % 2 == 0)
		{
			enemy->direction = (rand() % 2) * 2 - 1;
			enemy->y_direction = 0;
		}
		else
		{
			enemy->direction = 0;
			enemy->y_direction = (rand() % 2) * 2 - 1;
		}
		enemy->move_counter = 0;
		if (enemy->direction > 0)
			enemy->state = MOVE_RIGHT;
		else if (enemy->direction < 0)
			enemy->state = MOVE_LEFT;
	}
	new_x = enemy->x + enemy->direction * ENEMY_SPEED;
	new_y = enemy->y + enemy->y_direction * ENEMY_SPEED;
	if (!check_wall_collisions(new_x, new_y))
	{
		enemy->x = new_x;
		enemy->y = new_y;
	}
	else
	{
		enemy->direction *= -1;
		enemy->y_direction *= -1;
		if (enemy->direction > 0)
			enemy->state = MOVE_RIGHT;
		else if (enemy->direction < 0)
			enemy->state = MOVE_LEFT;
	}
	if (enemy->y < 0)
	{
		enemy->y = 0;
		enemy->y_direction = -enemy->y_direction;
	}
	if (enemy->y > get_game()->window_height - SPRITE_SIZE)
	{
		enemy->y = get_game()->window_height - SPRITE_SIZE;
		enemy->y_direction = -enemy->y_direction;
	}
	if (enemy->state == MOVE_RIGHT)
		current_anim = &enemy->move_right;
	else
		current_anim = &enemy->move_left;
	update_animation(current_anim);
}

static void	draw_enemy(void)
{
	t_enemy		*enemy;
	t_animation	*current_anim;

	enemy = &get_game()->enemy;
	if (enemy->is_dead)
		return ;
	if (enemy->invincibility_frames > 0)
	{
		enemy->is_visible = !enemy->is_visible;
		if (!enemy->is_visible)
			return ;
	}
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
	while (++y <= get_game()->window_height / 40)
	{
		x = -1;
		while (++x <= (get_game()->window_width / 40))
		{
			if ((x + y) % 2 == 0)
				draw_image(&get_game()->floor, &get_game()->canvas, x * 40, y
					* 40);
			else
				draw_image(&get_game()->floor2, &get_game()->canvas, x * 40, y
					* 40);
		}
	}
}

static void draw_collectibles(void)
{
	t_collectible	*collectible;
	int				i;

	collectible = &get_game()->collectible;
	i = -1;
	while (++i < collectible->count)
	{
		if (!collectible->collected[i])
		{
			collectible->base.x = collectible->x_positions[i];
			collectible->base.y = collectible->y_positions[i];
			draw_animation(&collectible->base, &get_game()->canvas);
		}
	}
}

static void	draw_walls(void)
{
	t_wall	*wall;
	int		i;

	wall = &get_game()->wall;
	update_animation(&wall->base);

	i = -1;
	while (++i < wall->count)
	{
		wall->base.x = wall->x_positions[i];
		wall->base.y = wall->y_positions[i];
		draw_animation(&wall->base, &get_game()->canvas);
	}
}

int	key_loop(int key, char *action)
{
	t_player	*player;

	player = &get_game()->player;
	if (key == XK_w)
		get_game()->move_up = (*action == 'p');
	if (key == XK_a)
		get_game()->move_left = (*action == 'p');
	if (key == XK_s)
		get_game()->move_down = (*action == 'p');
	if (key == XK_d)
		get_game()->move_right = (*action == 'p');
	if (key == XK_space && *action == 'p' && !player->is_attacking && player->attack_cooldown == 0)
	{
        player->is_attacking = true;
		player->attack_frame = 0;
        if (player->state == IDLE_RIGHT || player->state == MOVE_RIGHT)
        {
			player->state = ATTACK_RIGHT;
			player->attack_right.current_frame = 0;
		}
        else
		{
            player->state = ATTACK_LEFT;
			player->attack_left.current_frame = 0;
		}
    }
	if (key == XK_Escape)
		exit_game();
	return (0);
}

static void	init_exit(void)
{
	t_exit	*exit;

	exit = &get_game()->exit;
	exit->sprite = make_sprite("assets/exit.xpm");
	exit->x = 600;
	exit->y = 100;
}

static void	draw_exit(void)
{
	t_exit	*exit;

	exit = &get_game()->exit;
	draw_image(&exit->sprite, &get_game()->canvas, exit->x, exit->y);
}

static void	init_health(void)
{
	t_health	*health;

	health = &get_game()->health;
	health->health1 = make_sprite("assets/health/health1.xpm");
	health->health2 = make_sprite("assets/health/health2.xpm");
	health->health3 = make_sprite("assets/health/health3.xpm");
}

static void	draw_health(void)
{
	t_health	*health;
	t_img		*current_sprite;

	health = &get_game()->health;
	if (get_game()->player.lives == 3)
		current_sprite = &health->health3;
	else if (get_game()->player.lives == 2)
		current_sprite = &health->health2;
	else if (get_game()->player.lives == 1)
		current_sprite = &health->health1;
	else
		return ;
	draw_image(current_sprite, &get_game()->canvas, 30, 0);
}

static void	init_player(void)
{
	t_player	*player;
	t_map		*map;
	int			i;
	int			j;

	player = &get_game()->player;
	map = &get_game()->map;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == 'P')
			{
				player->x = j * SPRITE_SIZE;
				player->y = i * SPRITE_SIZE;
				break ;
			}
		}
	}
	get_game()->move_count = 0;
	player->idle_right.sprites[0] = make_sprite("assets/player/idle_right/player_idle_right00.xpm");
	player->idle_right.sprites[1] = make_sprite("assets/player/idle_right/player_idle_right01.xpm");
	init_animation(&player->idle_right, FRAME_COUNT, PLAYER_IDLE_ANIMATION_SPEED);
	player->idle_left.sprites[0] = make_sprite("assets/player/idle_left/player_idle_left00.xpm");
	player->idle_left.sprites[1] = make_sprite("assets/player/idle_left/player_idle_left01.xpm");
	init_animation(&player->idle_left, FRAME_COUNT, PLAYER_IDLE_ANIMATION_SPEED);
	player->move_right.sprites[0] = make_sprite("assets/player/move_right/player_move_right00.xpm");
	player->move_right.sprites[1] = make_sprite("assets/player/move_right/player_move_right01.xpm");
	init_animation(&player->move_right, FRAME_COUNT, PLAYER_MOVE_AND_ATTACK_ANIMATION_SPEED);
	player->move_left.sprites[0] = make_sprite("assets/player/move_left/player_move_left00.xpm");
	player->move_left.sprites[1] = make_sprite("assets/player/move_left/player_move_left01.xpm");
	init_animation(&player->move_left, FRAME_COUNT, PLAYER_MOVE_AND_ATTACK_ANIMATION_SPEED);
	player->attack_right.sprites[0] = make_sprite("assets/player/attack_right/player_attack_right00.xpm");
	player->attack_right.sprites[1] = make_sprite("assets/player/attack_right/player_attack_right01.xpm");
	init_animation(&player->attack_right, FRAME_COUNT, PLAYER_MOVE_AND_ATTACK_ANIMATION_SPEED);
	player->attack_left.sprites[0] = make_sprite("assets/player/attack_left/player_attack_left00.xpm");
	player->attack_left.sprites[1] = make_sprite("assets/player/attack_left/player_attack_left01.xpm");
	init_animation(&player->attack_left, FRAME_COUNT, PLAYER_MOVE_AND_ATTACK_ANIMATION_SPEED);
	player->state = IDLE_RIGHT;
	player->last_direction = MOVE_RIGHT;
	player->lives = 3;
	player->invincibility_frames = 0;
	player->is_visible = 1;
	player->attack_cooldown = 0;
	player->max_attack_cooldown = ATTACK_COOLDOWN;
	player->is_attacking = false;
	player->attack_frame = 0;
}

static void	update_player_position(void)
{
	int			prev_x;
	int			prev_y;
	t_player	*player;
	static int	move_counter = 0;

	player = &get_game()->player;
	prev_x = player->x;
	prev_y = player->y;
	if (!player->is_attacking)
	{
		if (get_game()->move_up || get_game()->move_down || get_game()->move_left
			|| get_game()->move_right)
		{
			if (get_game()->move_up)
				player->y -= PLAYER_SPEED;
			if (get_game()->move_down)
				player->y += PLAYER_SPEED;
			if (get_game()->move_left)
				player->x -= PLAYER_SPEED;
			if (get_game()->move_right)
				player->x += PLAYER_SPEED;
		}
		check_bounds(player);
	}
	if (check_wall_collisions(player->x, player->y))
	{
		player->x = prev_x;
		player->y = prev_y;
	}
	else if (prev_x != player->x || prev_y != player->y)
	{
		move_counter++;
		if (move_counter >= 10)
		{
			get_game()->move_count++;
			ft_printf("Moves: %d\n", get_game()->move_count);
			move_counter = 0;
		}
	}
}

static void	update_player(void)
{
	t_player	*player;
	t_animation	*current_anim;

	player = &get_game()->player;

	if (player->attack_cooldown > 0)
		player->attack_cooldown--;
	if (player->is_attacking)
	{
		player->attack_timer++;
		if (player->state == ATTACK_RIGHT)
			current_anim = &player->attack_right;
		else
			current_anim = &player->attack_left;
		if (player->attack_timer < 10)
			current_anim->current_frame = 0;
		else if (player->attack_timer < 20)
			current_anim->current_frame = 1;
		else
		{
			player->is_attacking = false;
			player->attack_cooldown = player->max_attack_cooldown;
			player->attack_timer = 0;
			if (player->state == ATTACK_RIGHT)
				player->state = IDLE_RIGHT;
			else
				player->state = IDLE_LEFT;
		}
		return;
	}
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
	else
	{
		if (player->last_direction == MOVE_RIGHT)
			player->state = IDLE_RIGHT;
		else
			player->state = IDLE_LEFT;
	}
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

static void	draw_player(void)
{
	t_player	*player;
	t_animation	*current_anim;
	int			x_offset;

	player = &get_game()->player;
	current_anim = NULL;
	x_offset = 0;
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
	else if (player->state == MOVE_LEFT)
		current_anim = &player->move_left;
	else if (player->state == ATTACK_RIGHT)
		current_anim = &player->attack_right;
	else if (player->state == ATTACK_LEFT)
	{
		current_anim = &player->attack_left;
		x_offset = -20;
	}
	current_anim->x = player->x + x_offset;
	current_anim->y = player->y;
	draw_animation(current_anim, &get_game()->canvas);
}

static void	fps_cap(void)
{
	static struct timeval	last_frame = {0, 0};
	struct timeval			current_time;
	long					elapsed_us;

	gettimeofday(&current_time, NULL);
	elapsed_us = (current_time.tv_sec - last_frame.tv_sec) * 1000000
		+ (current_time.tv_usec - last_frame.tv_usec);
	if (elapsed_us < FRAME_DELAY)
		usleep(FRAME_DELAY - elapsed_us);
	last_frame = current_time;
}

static void handle_game_state(void)
{
    t_player *player;
    t_enemy *enemy;

    player = &get_game()->player;
    enemy = &get_game()->enemy;

    if (player->invincibility_frames > 0)
        player->invincibility_frames--;

    if (!enemy->is_dead && 
        check_collision(player->x, player->y, enemy->x, enemy->y,
                       ENEMY_COLLISION_WIDTH, ENEMY_COLLISION_HEIGHT) && 
        player->invincibility_frames == 0)
    {
        player->lives--;
        player->invincibility_frames = INVINCIBILITY_DURATION;
        if (player->lives <= 0)
        {
            ft_printf("Game Over\n");
            exit_game();
        }
    }
}

int	game_loop(void)
{
	char	*print_move;

	fps_cap();
	handle_game_state();
	draw_floor();
	update_collectible();
	draw_collectibles();
	draw_walls();
	if (!get_game()->enemy.is_dead)
	{
		update_enemy();
		check_attack_collision();		
		draw_enemy();
	}
	draw_exit();
	update_animation(&get_game()->wall.base);
	draw_animation(&get_game()->wall.base, &get_game()->canvas);
	update_player_position();
	update_player();
	draw_player();
	draw_health();
	if (get_game()->collectible_count == get_game()->collectible.count
		&& check_collision(get_game()->player.x, get_game()->player.y,
			get_game()->exit.x, get_game()->exit.y, 20, 20))
	{
		ft_printf("Victory!\n");
		exit_game();
	}
	draw_text_background();
	print_move = ft_itoa(get_game()->move_count);
	mlx_put_image_to_window(get_game()->mlx, get_game()->win,
		get_game()->canvas.img, 0, 0);
	mlx_string_put(get_game()->mlx, get_game()->win, 30, get_game()->window_height - 18,
		0x00FFFFFF, "MOVES:");
	mlx_string_put(get_game()->mlx, get_game()->win, 70, get_game()->window_height - 18,
		0x00FFFFFF, print_move);
	free(print_move);
	return (0);
}

static void	init_window(void)
{
	t_game	*game;

	game = get_game();
	game->window_width = game->map.width * SPRITE_SIZE;
	game->window_height = game->map.height * SPRITE_SIZE;
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, game->window_width,
			game->window_height, "so_long");
	if (!game->mlx || !game->win)
		exit_error();
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

t_map *parse_map(char *filename)
{
    t_map *map_info;
    char *line;
    int fd;
    int i;
    map_info = malloc(sizeof(t_map));
    if (!map_info)
        return (NULL);
    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        free(map_info);
        return (NULL);
    }
    map_info->height = 0;
    map_info->width = 0;
    while ((line = get_next_line(fd)) != NULL)
    {
        if (map_info->height == 0)
            map_info->width = ft_strlen(line) - 1;
        map_info->height++;
        free(line);
    }
    close(fd);
    map_info->map = malloc(sizeof(char *) * (map_info->height + 1));
    if (!map_info->map)
    {
        free(map_info);
        return (NULL);
    }
    fd = open(filename, O_RDONLY);
    i = 0;
    while ((line = get_next_line(fd)) != NULL)
    {
        map_info->map[i] = line;
        i++;
    }
    map_info->map[i] = NULL;
    close(fd);

    map_info->collectibles = 0;
    map_info->collectibles_reachable = 0;
    map_info->exit_reachable = 0;

    return (map_info);
}

int main(int argc, char **argv)
{
    t_map *map_info;

    if (argc != 2)
        return (ft_printf("Error\nUsage: ./so_long [map.ber]\n"));
    map_info = parse_map(argv[1]);
    if (!map_info)
		return (ft_printf("Error\nInvalid map\n"));
	get_game()->map = *map_info;
    init_window();
    init_sprites();
    setup_hooks();
    mlx_loop(get_game()->mlx);
    return (0);
}

// int	main(void)
// {
// 	init_window();
// 	init_sprites();
// 	setup_hooks();
// 	mlx_loop(get_game()->mlx);
// }
