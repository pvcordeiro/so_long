/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:14:46 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/18 22:00:39 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

t_game	*get_game(void)
{
	static t_game	game;

	return (&game);
}

int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

void	update_entities(void)
{
	update_mushroom();
	update_collectible();
	update_enemy();
	check_attack_collision();
	update_player_position();
	update_player();
}

void	draw_text_layer(void)
{
	t_game	*game;
	char	*count_str;

	game = get_game();
	if (game->vic || game->game_over)
		return ;
	count_str = ft_itoa(game->move_count);
	mlx_string_put(game->mlx, game->win, 25, game->window_height - 45, 0x000000,
		"MOVES");
	mlx_string_put(game->mlx, game->win, 33, game->window_height - 30, 0x000000,
		count_str);
	free(count_str);
	count_str = ft_itoa(game->collectible_count);
	mlx_string_put(game->mlx, game->win, game->window_width - 34, 45, 0x000000,
		count_str);
	free(count_str);
}

void	draw_frame(void)
{
	t_game	*game;

	game = get_game();
	draw_background_layer();
	if (!game->vic && !game->game_over)
	{
		draw_entity_layer();
		draw_player_and_exit();
		draw_ui_layer();
	}
	draw_end_game_screen();
}

void	update_game_state(void)
{
	if (get_game()->vic || get_game()->game_over)
		return ;
	handle_game_state();
	update_entities();
	victory_check();
}

void	draw_background_layer(void)
{
	draw_floor();
	draw_walls();
	update_animation(&get_game()->wall.base);
	draw_animation(&get_game()->wall.base, &get_game()->canvas);
}

void	draw_entity_layer(void)
{
	draw_mushroom();
	draw_collectibles();
	draw_enemy();
}

void	draw_player_and_exit(void)
{
	if (get_game()->player.y < get_game()->exit.y)
	{
		draw_player();
		draw_exit_full();
	}
	else
	{
		draw_exit_bottom();
		draw_player();
		draw_exit_top();
	}
}

void	draw_ui_layer(void)
{
	helper_message();
	draw_ui_banners();
	draw_sprint_icon();
	draw_health();
	draw_collectible_counter();
}

void	init_enemy_animations(t_enemy *enemy)
{
	load_animation_sprites(&enemy->move_right,
		"assets/enemy/move_right/enemy_move_right00.xpm",
		"assets/enemy/move_right/enemy_move_right01.xpm",
		ENEMY_ANIMATION_SPEED);
	load_animation_sprites(&enemy->move_left,
		"assets/enemy/move_left/enemy_move_left00.xpm",
		"assets/enemy/move_left/enemy_move_left01.xpm", ENEMY_ANIMATION_SPEED);
}

void	init_enemy_state(t_enemy *enemy, int x, int y)
{
	enemy->x = x * SPRITE_SIZE;
	enemy->y = y * SPRITE_SIZE;
	enemy->direction = (rand() % 2) * 2 - 1;
	enemy->y_direction = 0;
	if (enemy->direction == 1)
		enemy->state = MOVE_RIGHT;
	else
		enemy->state = MOVE_LEFT;
	enemy->move_counter = 0;
	enemy->lives = 3;
	enemy->is_dead = false;
	enemy->invincibility_frames = 0;
	enemy->is_visible = true;
}

static void	update_enemy_direction(t_enemy *enemy)
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
}

static void	handle_enemy_collision(t_enemy *enemy, int prev_x, int prev_y)
{
	if (check_wall_collisions(enemy->x + ENEMY_HITBOX_X_OFFSET, enemy->y
			+ ENEMY_HITBOX_Y_OFFSET, ENEMY_COLLISION_WIDTH,
			ENEMY_COLLISION_HEIGHT) || check_enemy_collisions(enemy->x,
			enemy->y, enemy - get_game()->enemy_list.enemies))
	{
		enemy->x = prev_x;
		enemy->y = prev_y;
		enemy->direction *= -1;
		enemy->y_direction *= -1;
	}
}

void	update_enemy_movement(t_enemy *enemy)
{
	int	prev_x;
	int	prev_y;

	enemy->move_counter++;
	if (enemy->move_counter >= ENEMY_MOVE_THRESHOLD)
		update_enemy_direction(enemy);
	prev_x = enemy->x;
	prev_y = enemy->y;
	enemy->x += enemy->direction * ENEMY_SPEED;
	enemy->y += enemy->y_direction * ENEMY_SPEED;
	handle_enemy_collision(enemy, prev_x, prev_y);
}

void	update_enemy_state(t_enemy *enemy)
{
	t_animation	*current_anim;

	if (enemy->is_dead)
		return ;
	if (enemy->invincibility_frames > 0)
		enemy->invincibility_frames--;
	update_enemy_movement(enemy);
	if (enemy->direction > 0)
		enemy->state = MOVE_RIGHT;
	else
		enemy->state = MOVE_LEFT;
	if (enemy->state == MOVE_RIGHT)
		current_anim = &enemy->move_right;
	else
		current_anim = &enemy->move_left;
	update_animation(current_anim);
}

static int	check_collision(t_coord pos1, t_coord pos2, int width, int height)
{
	return (pos1.x < (pos2.x + width) && (pos1.x + width) > pos2.x
		&& pos1.y < (pos2.y + height) && (pos1.y + height) > pos2.y);
}

int	check_enemy_collisions(int x, int y, int current_enemy)
{
	t_enemy_list	*enemy_list;
	int				i;
	t_coord			pos1;
	t_coord			pos2;

	enemy_list = &get_game()->enemy_list;
	i = -1;
	pos1.x = x + ENEMY_HITBOX_X_OFFSET;
	pos1.y = y + ENEMY_HITBOX_Y_OFFSET;
	while (++i < enemy_list->count)
	{
		if (i != current_enemy && !enemy_list->enemies[i].is_dead)
		{
			pos2.x = enemy_list->enemies[i].x + ENEMY_HITBOX_X_OFFSET;
			pos2.y = enemy_list->enemies[i].y + ENEMY_HITBOX_Y_OFFSET;
			if (check_collision(pos1, pos2, ENEMY_COLLISION_WIDTH,
					ENEMY_COLLISION_HEIGHT))
				return (1);
		}
	}
	return (0);
}

static bool	is_valid_attack_target(t_player *player, t_enemy *enemy)
{
	return (!enemy->is_dead && player->is_attacking
		&& enemy->invincibility_frames == 0 && ((player->state == ATTACK_RIGHT
				&& enemy->x > player->x) || (player->state == ATTACK_LEFT
				&& enemy->x < player->x)));
}

static void	apply_damage_to_enemy(t_enemy *enemy, t_player *player)
{
	enemy->lives--;
	enemy->invincibility_frames = INVINCIBILITY_DURATION;
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
	if (enemy->lives <= 0)
		enemy->is_dead = true;
}

void	check_attack_collision(void)
{
	t_player		*player;
	t_enemy_list	*enemy_list;
	int				i;
	t_coord			pos1;
	t_coord			pos2;

	player = &get_game()->player;
	enemy_list = &get_game()->enemy_list;
	pos1.x = player->x;
	pos1.y = player->y + ATTACK_HITBOX_Y_OFFSET;
	i = -1;
	while (++i < enemy_list->count)
	{
		if (is_valid_attack_target(player, &enemy_list->enemies[i]))
		{
			pos2.x = enemy_list->enemies[i].x + ENEMY_HITBOX_X_OFFSET;
			pos2.y = enemy_list->enemies[i].y + ENEMY_HITBOX_Y_OFFSET;
			if (check_collision(pos1, pos2, ATTACK_RANGE,
					ENEMY_COLLISION_HEIGHT))
			{
				apply_damage_to_enemy(&enemy_list->enemies[i], player);
			}
		}
	}
}

unsigned int	*get_pixel(t_img *data, int x, int y)
{
	return ((unsigned int *)(data->addr + (y * data->line_len + x * (data->bpp
				/ 8))));
}

static void	cleanup_dynamic_memory(t_game *game)
{
	free(game->wall.x_positions);
	free(game->wall.y_positions);
	free(game->collectible.x_positions);
	free(game->collectible.y_positions);
	free(game->collectible.collected);
}

static void	cleanup_enemy_sprites(t_game *game)
{
	int	i;
	int	j;

	i = -1;
	while (++i < game->enemy_list.count)
	{
		j = -1;
		while (++j < 2)
		{
			mlx_destroy_image(game->mlx,
				game->enemy_list.enemies[i].move_right.sprites[j].img);
			mlx_destroy_image(game->mlx,
				game->enemy_list.enemies[i].move_left.sprites[j].img);
		}
	}
	free(game->enemy_list.enemies);
}

static void	cleanup_collectible_sprites(t_game *game)
{
	int	i;

	i = -1;
	while (++i < 4)
		mlx_destroy_image(game->mlx, game->collectible.base.sprites[i].img);
}

static void	cleanup_player_sprites(t_game *game)
{
	int	i;

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
	}
}

static void	cleanup_misc_sprites(t_game *game)
{
	mlx_destroy_image(game->mlx, game->canvas.img);
	mlx_destroy_image(game->mlx, game->floor.img);
	mlx_destroy_image(game->mlx, game->floor2.img);
	mlx_destroy_image(game->mlx, game->mushroom.sprite.img);
	mlx_destroy_image(game->mlx, game->exit.sprite.img);
	mlx_destroy_image(game->mlx, game->health.health1.img);
	mlx_destroy_image(game->mlx, game->health.health2.img);
	mlx_destroy_image(game->mlx, game->health.health3.img);
	mlx_destroy_image(game->mlx, game->health.sprint.img);
	mlx_destroy_image(game->mlx, game->health.banner.img);
	mlx_destroy_image(game->mlx, game->health.message.img);
}

void	cleanup_sprites(void)
{
	t_game	*game;

	game = get_game();
	cleanup_dynamic_memory(game);
	cleanup_enemy_sprites(game);
	cleanup_collectible_sprites(game);
	cleanup_player_sprites(game);
	cleanup_misc_sprites(game);
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
	t_game	*game;

	game = get_game();
	if (game->mlx)
	{
		cleanup_sprites();
		mlx_destroy_window(game->mlx, game->win);
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
	ft_printf("Error\n");
	exit(EXIT_FAILURE);
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
			if (!(sx + x > get_game()->window_width || sy
					+ y > get_game()->window_height || sx + x < 0 || sy + y < 0)
				&& (*get_pixel(src, sx, sy) != 0xFF000000))
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

void	update_collectible(void)
{
	int				i;
	t_collectible	*collectible;
	t_player		*player;
	t_coord			player_pos;
	t_coord			collect_pos;

	collectible = &get_game()->collectible;
	player = &get_game()->player;
	player_pos.x = player->x;
	player_pos.y = player->y;
	i = -1;
	while (++i < collectible->count)
	{
		collect_pos.x = collectible->x_positions[i] + HITBOX_X_OFFSET;
		collect_pos.y = collectible->y_positions[i] + HITBOX_Y_OFFSET;
		if (check_collision(player_pos, collect_pos, COLLECTIBLE_SIZE,
				COLLECTIBLE_SIZE))
		{
			collectible->collected[i] = 1;
			get_game()->collectible_count++;
		}
	}
	update_animation(&collectible->base);
}

static int	count_collectible_positions(t_map *map)
{
	int	count;
	int	i;
	int	j;

	count = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
			if (map->map[i][j] == 'C')
				count++;
	}
	return (count);
}

static bool	allocate_collectible_arrays(t_collectible *collectible, int count)
{
	collectible->x_positions = malloc(sizeof(int) * count);
	collectible->y_positions = malloc(sizeof(int) * count);
	collectible->collected = malloc(sizeof(int) * count);
	if (!collectible->x_positions || !collectible->y_positions
		|| !collectible->collected)
		return (false);
	return (true);
}

static void	store_collectible_positions(t_collectible *collectible, t_map *map)
{
	int	i;
	int	j;
	int	count;

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
}

static void	load_collectible_sprites(t_collectible *collectible)
{
	collectible->base.sprites[0] = make_sprite("assets/collect/collect00.xpm");
	collectible->base.sprites[1] = make_sprite("assets/collect/collect01.xpm");
	collectible->base.sprites[2] = make_sprite("assets/collect/collect02.xpm");
	collectible->base.sprites[3] = make_sprite("assets/collect/collect03.xpm");
	collectible->base.sprites[4] = make_sprite("assets/ban_col.xpm");
	init_animation(&collectible->base, 4, COLLECTIBLE_ANIMATION_SPEED);
}

void	init_collectible(void)
{
	t_collectible	*collectible;
	t_map			*map;
	int				count;

	collectible = &get_game()->collectible;
	map = &get_game()->map;
	count = count_collectible_positions(map);
	collectible->count = count;
	if (!allocate_collectible_arrays(collectible, count))
		exit_error();
	store_collectible_positions(collectible, map);
	load_collectible_sprites(collectible);
	get_game()->collectible_count = 0;
}

static void	init_wall(void)
{
	t_wall	*wall;
	t_map	*map;
	int		count;
	int		i;

	wall = &get_game()->wall;
	map = &get_game()->map;
	count = count_map_char(map->map, map->height, map->width, '1');
	wall->count = count;
	wall->x_positions = malloc(sizeof(int) * count);
	wall->y_positions = malloc(sizeof(int) * count);
	count = 0;
	i = -1;
	while (++i < map->height * map->width)
	{
		if (map->map[i / map->width][i % map->width] == '1')
		{
			wall->x_positions[count] = (i % map->width) * SPRITE_SIZE;
			wall->y_positions[count] = (i / map->width) * SPRITE_SIZE;
			count++;
		}
	}
	load_animation_sprites(&wall->base, "assets/terrain/lava00.xpm",
		"assets/terrain/lava01.xpm", WALL_ANIMATION_SPEED);
}

int	check_wall_collisions(int x, int y, int width, int height)
{
	t_wall	*wall;
	int		i;
	t_coord	pos1;
	t_coord	pos2;

	wall = &get_game()->wall;
	pos1.x = x;
	pos1.y = y;
	i = -1;
	while (++i < wall->count)
	{
		pos2.x = wall->x_positions[i];
		pos2.y = wall->y_positions[i];
		if (check_collision(pos1, pos2, width, height))
			return (1);
	}
	return (0);
}

void	init_entity_position(t_map *map, char c, int *x, int *y)
{
	int	i;
	int	j;

	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == c)
			{
				*x = j * SPRITE_SIZE;
				*y = i * SPRITE_SIZE;
				return ;
			}
		}
	}
}

void	handle_entity_collision(int *x, int *y, int prev_x, int prev_y)
{
	if (check_wall_collisions(*x + PLAYER_HITBOX_X_OFFSET, *y
			+ PLAYER_HITBOX_Y_OFFSET, PLAYER_COLLISION_WIDTH,
			PLAYER_COLLISION_HEIGHT))
	{
		*x = prev_x;
		*y = prev_y;
	}
}

void	handle_player_movement(t_player *player, int movement_speed)
{
	int	prev_x;
	int	prev_y;

	prev_x = player->x;
	prev_y = player->y;
	if (get_game()->move_left)
		player->x -= movement_speed;
	if (get_game()->move_right)
		player->x += movement_speed;
	handle_entity_collision(&player->x, &player->y, prev_x, prev_y);
	if (get_game()->move_up)
		player->y -= movement_speed;
	if (get_game()->move_down)
		player->y += movement_speed;
	handle_entity_collision(&player->x, &player->y, prev_x, prev_y);
}

void	handle_enemy_movement(t_enemy *enemy)
{
	int	prev_x;
	int	prev_y;

	prev_x = enemy->x;
	prev_y = enemy->y;
	enemy->x += enemy->direction * ENEMY_SPEED;
	enemy->y += enemy->y_direction * ENEMY_SPEED;
	if (check_wall_collisions(enemy->x + ENEMY_HITBOX_X_OFFSET, enemy->y
			+ ENEMY_HITBOX_Y_OFFSET, ENEMY_COLLISION_WIDTH,
			ENEMY_COLLISION_HEIGHT))
	{
		enemy->x = prev_x;
		enemy->y = prev_y;
		enemy->direction *= -1;
		enemy->y_direction *= -1;
	}
}

static bool	allocate_enemy_array(t_enemy_list *enemy_list, int count)
{
	enemy_list->enemies = malloc(sizeof(t_enemy) * count);
	if (!enemy_list->enemies)
		return (false);
	enemy_list->count = count;
	return (true);
}

static void	init_single_enemy(t_enemy *enemy, int x, int y)
{
	init_enemy_animations(enemy);
	init_enemy_state(enemy, x, y);
}

static void	setup_enemies(t_enemy_list *enemy_list, t_map *map)
{
	int	i;
	int	j;
	int	count;

	count = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == 'X')
				init_single_enemy(&enemy_list->enemies[count++], j, i);
		}
	}
}

void	init_enemy(void)
{
	t_enemy_list	*enemy_list;
	t_map			*map;
	int				count;

	enemy_list = &get_game()->enemy_list;
	map = &get_game()->map;
	count = count_map_char(map->map, map->height, map->width, 'X');
	if (!allocate_enemy_array(enemy_list, count))
		exit_error();
	setup_enemies(enemy_list, map);
}

void	update_enemy(void)
{
	t_enemy_list	*enemy_list;
	int				i;

	enemy_list = &get_game()->enemy_list;
	i = -1;
	while (++i < enemy_list->count)
		update_enemy_state(&enemy_list->enemies[i]);
}

static bool	is_enemy_visible(t_enemy *enemy)
{
	if (enemy->invincibility_frames > 0)
	{
		enemy->is_visible = !enemy->is_visible;
		return (enemy->is_visible);
	}
	return (true);
}

static t_animation	*get_enemy_animation(t_enemy *enemy)
{
	if (enemy->state == MOVE_RIGHT)
		return (&enemy->move_right);
	return (&enemy->move_left);
}

static void	draw_single_enemy(t_enemy *enemy)
{
	t_animation	*current_anim;

	if (!enemy->is_dead && is_enemy_visible(enemy))
	{
		current_anim = get_enemy_animation(enemy);
		current_anim->x = enemy->x;
		current_anim->y = enemy->y;
		draw_animation(current_anim, &get_game()->canvas);
	}
}

void	draw_enemy(void)
{
	t_enemy_list	*enemy_list;
	int				i;

	enemy_list = &get_game()->enemy_list;
	i = -1;
	while (++i < enemy_list->count)
		draw_single_enemy(&enemy_list->enemies[i]);
}

void	draw_floor(void)
{
	int	x;
	int	y;

	y = -1;
	while (++y <= get_game()->window_height / SPRITE_SIZE)
	{
		x = -1;
		while (++x <= (get_game()->window_width / SPRITE_SIZE))
		{
			if ((x + y) % 2 == 0)
				draw_image(&get_game()->floor, &get_game()->canvas, x
					* SPRITE_SIZE, y * SPRITE_SIZE);
			else
				draw_image(&get_game()->floor2, &get_game()->canvas, x
					* SPRITE_SIZE, y * SPRITE_SIZE);
		}
	}
}

void	draw_collectibles(void)
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

void	draw_walls(void)
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

static void	handle_movement_keys(t_game *game, int key, char *action)
{
	if (key == XK_w)
		game->move_up = (*action == 'p');
	if (key == XK_a)
		game->move_left = (*action == 'p');
	if (key == XK_s)
		game->move_down = (*action == 'p');
	if (key == XK_d)
		game->move_right = (*action == 'p');
}

static void	handle_sprint_key(t_game *game, char *action)
{
	if (*action == 'p' && game->player.can_sprint)
		game->player.is_sprinting = true;
	else if (*action == 'r')
		game->player.is_sprinting = false;
}

static void	handle_attack_key(t_player *player)
{
	if (!player->is_attacking && player->attack_cooldown == 0)
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
}

int	key_loop(int key, char *action)
{
	t_game	*game;

	game = get_game();
	if (key == XK_Escape)
		exit_game();
	if (!game->vic && !game->game_over)
	{
		if (key == XK_Shift_L)
			handle_sprint_key(game, action);
		handle_movement_keys(game, key, action);
		if (key == XK_space && *action == 'p')
			handle_attack_key(&game->player);
	}
	return (0);
}

static void	init_exit(void)
{
	t_exit	*exit;
	t_map	*map;
	int		i;
	int		j;

	exit = &get_game()->exit;
	map = &get_game()->map;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == 'E')
			{
				exit->x = j * SPRITE_SIZE;
				exit->y = i * SPRITE_SIZE;
				break ;
			}
		}
	}
	exit->sprite = make_sprite("assets/exit.xpm");
}

void	draw_exit_bottom(void)
{
	t_exit	*exit;
	t_img	temp;
	int		i;
	int		j;
	int		draw_y;

	exit = &get_game()->exit;
	temp = exit->sprite;
	draw_y = exit->y - (exit->sprite.height - SPRITE_SIZE);
	i = temp.height / 2;
	while (i < temp.height)
	{
		j = -1;
		while (++j < temp.width)
		{
			if (*get_pixel(&temp, j, i) != 0xFF000000)
				*get_pixel(&get_game()->canvas, exit->x + j, draw_y
					+ i) = *get_pixel(&temp, j, i);
		}
		i++;
	}
}

void	draw_exit_top(void)
{
	t_exit	*exit;
	t_img	temp;
	int		i;
	int		j;
	int		draw_y;

	exit = &get_game()->exit;
	temp = exit->sprite;
	draw_y = exit->y - (exit->sprite.height - SPRITE_SIZE);
	i = -1;
	while (++i < temp.height / 2)
	{
		j = -1;
		while (++j < temp.width)
		{
			if (*get_pixel(&temp, j, i) != 0xFF000000)
				*get_pixel(&get_game()->canvas, exit->x + j, draw_y
					+ i) = *get_pixel(&temp, j, i);
		}
	}
}

void	draw_exit_full(void)
{
	t_exit	*exit;
	int		draw_y;

	exit = &get_game()->exit;
	draw_y = exit->y - (exit->sprite.height - SPRITE_SIZE);
	draw_image(&exit->sprite, &get_game()->canvas, exit->x, draw_y);
}

void	init_health(void)
{
	t_health	*health;

	health = &get_game()->health;
	health->health1 = make_sprite("assets/health/health1.xpm");
	health->health2 = make_sprite("assets/health/health2.xpm");
	health->health3 = make_sprite("assets/health/health3.xpm");
	health->sprint = make_sprite("assets/sprint.xpm");
	health->banner = make_sprite("assets/banner.xpm");
	health->message = make_sprite("assets/helper_message.xpm");
}

void	draw_health(void)
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
	draw_image(current_sprite, &get_game()->canvas, 10, -10);
}

static int	count_valid_positions(t_map *map)
{
	int	valid_positions;
	int	i;
	int	j;

	valid_positions = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
			if (map->map[i][j] == '0')
				valid_positions++;
	}
	return (valid_positions);
}

static void	find_random_position(t_mushroom *mushroom, t_map *map,
		int random_position)
{
	int	current_position;
	int	i;
	int	j;

	current_position = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == '0')
			{
				if (current_position == random_position)
				{
					mushroom->x = j * SPRITE_SIZE;
					mushroom->y = i * SPRITE_SIZE;
					return ;
				}
				current_position++;
			}
		}
	}
}

void	init_mushroom(void)
{
	t_mushroom	*mushroom;
	t_map		*map;
	int			valid_positions;

	mushroom = &get_game()->mushroom;
	map = &get_game()->map;
	mushroom->sprite = make_sprite("assets/mushroom.xpm");
	mushroom->active = true;
	mushroom->collected = false;
	valid_positions = count_valid_positions(map);
	find_random_position(mushroom, map, rand() % valid_positions);
}

void	update_mushroom(void)
{
	t_mushroom	*mushroom;
	t_player	*player;
	t_coord		pos1;
	t_coord		pos2;

	mushroom = &get_game()->mushroom;
	player = &get_game()->player;
	if (!mushroom->active || mushroom->collected)
		return ;
	pos1.x = player->x;
	pos1.y = player->y;
	pos2.x = mushroom->x + HITBOX_X_OFFSET;
	pos2.y = mushroom->y + HITBOX_Y_OFFSET;
	if (check_collision(pos1, pos2, COLLECTIBLE_SIZE, COLLECTIBLE_SIZE))
	{
		if (player->lives < 3)
		{
			player->lives++;
			mushroom->collected = true;
			mushroom->active = false;
		}
	}
}

void	draw_mushroom(void)
{
	t_mushroom	*mushroom;

	mushroom = &get_game()->mushroom;
	if (mushroom->active && !mushroom->collected)
	{
		draw_image(&mushroom->sprite, &get_game()->canvas, mushroom->x,
			mushroom->y);
	}
}

int	count_map_char(char **map, int height, int width, char c)
{
	int	i;
	int	j;
	int	count;

	count = 0;
	i = -1;
	while (++i < height)
	{
		j = -1;
		while (++j < width)
			if (map[i][j] == c)
				count++;
	}
	return (count);
}

void	load_animation_sprites(t_animation *anim, char *path1, char *path2,
		int speed)
{
	anim->sprites[0] = make_sprite(path1);
	anim->sprites[1] = make_sprite(path2);
	init_animation(anim, FRAME_COUNT, speed);
}

void	load_player_animations(t_player *player)
{
	load_animation_sprites(&player->idle_right,
		"assets/player/idle_right/player_idle_right00.xpm",
		"assets/player/idle_right/player_idle_right01.xpm",
		PLAYER_IDLE_ANIMATION_SPEED);
	load_animation_sprites(&player->idle_left,
		"assets/player/idle_left/player_idle_left00.xpm",
		"assets/player/idle_left/player_idle_left01.xpm",
		PLAYER_IDLE_ANIMATION_SPEED);
	load_animation_sprites(&player->move_right,
		"assets/player/move_right/player_move_right00.xpm",
		"assets/player/move_right/player_move_right01.xpm",
		PLAYER_IDLE_ANIMATION_SPEED);
	load_animation_sprites(&player->move_left,
		"assets/player/move_left/player_move_left00.xpm",
		"assets/player/move_left/player_move_left01.xpm",
		PLAYER_IDLE_ANIMATION_SPEED);
	load_animation_sprites(&player->attack_right,
		"assets/player/attack_right/player_attack_right00.xpm",
		"assets/player/attack_right/player_attack_right01.xpm",
		PLAYER_IDLE_ANIMATION_SPEED);
	load_animation_sprites(&player->attack_left,
		"assets/player/attack_left/player_attack_left00.xpm",
		"assets/player/attack_left/player_attack_left01.xpm",
		PLAYER_IDLE_ANIMATION_SPEED);
}

static void	init_player_position(t_player *player, t_map *map)
{
	init_entity_position(map, 'P', &player->x, &player->y);
	player->state = IDLE_RIGHT;
	player->last_direction = MOVE_RIGHT;
}

static void	init_player_stats(t_player *player)
{
	player->lives = 3;
	player->invincibility_frames = 0;
	player->is_visible = 1;
}

static void	init_player_combat(t_player *player)
{
	player->attack_cooldown = 0;
	player->max_attack_cooldown = ATTACK_COOLDOWN;
	player->is_attacking = false;
	player->attack_frame = 0;
}

static void	init_player_movement(t_player *player)
{
	player->is_sprinting = false;
	player->sprint_duration = 0;
	player->sprint_cooldown = 0;
	player->can_sprint = true;
}

void	init_player(void)
{
	t_player	*player;
	t_map		*map;

	player = &get_game()->player;
	map = &get_game()->map;
	init_player_position(player, map);
	init_player_stats(player);
	init_player_combat(player);
	init_player_movement(player);
	load_player_animations(player);
}

static void	handle_sprint(t_player *player, int *movement_speed)
{
	if (player->sprint_duration < SPRINT_DURATION)
	{
		*movement_speed *= SPRINT_MULTIPLIER;
		player->sprint_duration++;
	}
	else
	{
		player->is_sprinting = false;
		player->can_sprint = false;
		player->sprint_duration = 0;
	}
}

static void	handle_sprint_cooldown(t_player *player)
{
	if (!player->can_sprint)
	{
		player->sprint_cooldown++;
		if (player->sprint_cooldown >= SPRINT_COOLDOWN)
		{
			player->can_sprint = true;
			player->sprint_cooldown = 0;
		}
	}
}

static void	handle_movement(t_player *player, int movement_speed, int *prev_x,
		int *prev_y)
{
	if (get_game()->move_left)
		player->x -= movement_speed;
	if (get_game()->move_right)
		player->x += movement_speed;
	handle_entity_collision(&player->x, &player->y, *prev_x, *prev_y);
	if (get_game()->move_up)
		player->y -= movement_speed;
	if (get_game()->move_down)
		player->y += movement_speed;
	handle_entity_collision(&player->x, &player->y, *prev_x, *prev_y);
}

void	update_player_position(void)
{
	int			prev_x;
	int			prev_y;
	t_player	*player;
	static int	distance_moved = 0;
	int			movement_speed;

	player = &get_game()->player;
	prev_x = player->x;
	prev_y = player->y;
	movement_speed = PLAYER_SPEED;
	if (player->is_sprinting)
		handle_sprint(player, &movement_speed);
	handle_sprint_cooldown(player);
	if (!player->is_attacking)
		handle_movement(player, movement_speed, &prev_x, &prev_y);
	if (prev_x != player->x || prev_y != player->y)
	{
		distance_moved += ft_abs(player->x - prev_x) + ft_abs(player->y
				- prev_y);
		if (distance_moved >= SPRITE_SIZE)
		{
			get_game()->move_count++;
			distance_moved = 0;
		}
	}
}

static void	handle_player_attack(t_player *player, t_animation *current_anim)
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
}

static void	update_player_state(t_player *player)
{
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
}

void	update_player(void)
{
	t_player	*player;
	t_animation	*current_anim;

	player = &get_game()->player;
	if (player->attack_cooldown > 0)
		player->attack_cooldown--;
	if (player->is_attacking)
	{
		handle_player_attack(player, current_anim);
		return ;
	}
	update_player_state(player);
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

static bool	is_player_visible(t_player *player)
{
	if (player->invincibility_frames > 0)
	{
		player->is_visible = !player->is_visible;
		return (player->is_visible);
	}
	return (true);
}

static t_animation	*get_player_animation(t_player *player)
{
	if (player->state == IDLE_RIGHT)
		return (&player->idle_right);
	if (player->state == IDLE_LEFT)
		return (&player->idle_left);
	if (player->state == MOVE_RIGHT)
		return (&player->move_right);
	if (player->state == MOVE_LEFT)
		return (&player->move_left);
	if (player->state == ATTACK_RIGHT)
		return (&player->attack_right);
	return (&player->attack_left);
}

void	draw_player(void)
{
	t_player	*player;
	t_animation	*current_anim;

	player = &get_game()->player;
	if (!is_player_visible(player))
		return ;
	current_anim = get_player_animation(player);
	current_anim->x = player->x;
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

static void	update_player_invincibility(t_player *player)
{
	if (player->invincibility_frames > 0)
		player->invincibility_frames--;
}

static bool	check_enemy_hit(t_player *player, t_enemy *enemy)
{
	t_coord	pos1;
	t_coord	pos2;

	pos1.x = player->x + PLAYER_HITBOX_X_OFFSET;
	pos1.y = player->y + PLAYER_HITBOX_Y_OFFSET;
	pos2.x = enemy->x + ENEMY_HITBOX_X_OFFSET;
	pos2.y = enemy->y + ENEMY_HITBOX_Y_OFFSET;
	return (check_collision(pos1, pos2, PLAYER_COLLISION_WIDTH,
			PLAYER_COLLISION_HEIGHT));
}

static void	handle_enemy_hit(t_player *player)
{
	player->lives--;
	player->invincibility_frames = INVINCIBILITY_DURATION;
	if (player->lives <= 0)
		get_game()->game_over = true;
}

void	handle_game_state(void)
{
	t_player		*player;
	t_enemy_list	*enemy_list;
	int				i;

	player = &get_game()->player;
	enemy_list = &get_game()->enemy_list;
	update_player_invincibility(player);
	i = -1;
	while (++i < enemy_list->count)
	{
		if (!enemy_list->enemies[i].is_dead
			&& player->invincibility_frames == 0)
		{
			if (check_enemy_hit(player, &enemy_list->enemies[i]))
			{
				handle_enemy_hit(player);
				break ;
			}
		}
	}
}

void	helper_message(void)
{
	int		x;
	int		y;
	t_coord	pos1;
	t_coord	pos2;

	pos1.x = get_game()->player.x;
	pos1.y = get_game()->player.y;
	pos2.x = get_game()->exit.x;
	pos2.y = get_game()->exit.y;
	if (check_collision(pos1, pos2, 40, 40)
		&& get_game()->collectible_count < get_game()->collectible.count)
	{
		x = get_game()->exit.x + (SPRITE_SIZE - 160) / 2;
		y = get_game()->exit.y - 40;
		draw_image(&get_game()->health.message, &get_game()->canvas, x, y);
	}
}

void	victory_check(void)
{
	t_coord	pos1;
	t_coord	pos2;

	pos1.x = get_game()->player.x;
	pos1.y = get_game()->player.y;
	pos2.x = get_game()->exit.x;
	pos2.y = get_game()->exit.y;
	if (get_game()->collectible_count == get_game()->collectible.count
		&& check_collision(pos1, pos2, 20, 20))
	{
		get_game()->vic = true;
	}
}

void	draw_collectible_counter(void)
{
	t_collectible	*collectible;
	int				x;
	int				y;

	x = get_game()->window_width - 80;
	y = 0;
	collectible = &get_game()->collectible;
	draw_image(&collectible->base.sprites[4], &get_game()->canvas, x, y);
}

void	draw_sprint_icon(void)
{
	t_player	*player;
	static int	flash_counter = 0;
	int			sprint_x;
	int			sprint_y;

	player = &get_game()->player;
	sprint_x = get_game()->window_width - 80;
	sprint_y = get_game()->window_height - 80;
	if (player->is_sprinting)
		draw_image(&get_game()->health.sprint, &get_game()->canvas, sprint_x,
			sprint_y);
	else if (!player->can_sprint)
	{
		flash_counter++;
		if ((flash_counter / 15) % 2)
			draw_image(&get_game()->health.sprint, &get_game()->canvas,
				sprint_x, sprint_y);
	}
	else
		draw_image(&get_game()->health.sprint, &get_game()->canvas, sprint_x,
			sprint_y);
}

void	draw_ui_banners(void)
{
	int	banner_top_right;
	int	banner_bottom_left;
	int	banner_bottom_right;

	banner_top_right = get_game()->window_width - 80;
	draw_image(&get_game()->health.banner, &get_game()->canvas,
		banner_top_right, 0);
	banner_bottom_left = 0;
	draw_image(&get_game()->health.banner, &get_game()->canvas,
		banner_bottom_left, get_game()->window_height - 80);
	banner_bottom_right = get_game()->window_width - 80;
	draw_image(&get_game()->health.banner, &get_game()->canvas,
		banner_bottom_right, get_game()->window_height - 80);
}

void	draw_end_game_screen(void)
{
	t_game	*game;
	int		x;
	int		y;

	game = get_game();
	if (game->vic || game->game_over)
	{
		x = (game->window_width - 400) / 2;
		y = (game->window_height - 400) / 2;
		if (game->vic)
			draw_image(&game->victory, &game->canvas, x, y);
		else
			draw_image(&game->defeat, &game->canvas, x, y);
	}
}

int	game_loop(void)
{
	fps_cap();
	update_game_state();
	draw_frame();
	mlx_put_image_to_window(get_game()->mlx, get_game()->win,
		get_game()->canvas.img, 0, 0);
	draw_text_layer();
	return (0);
}

void	init_game(char *map_path)
{
	t_game	*game;
	t_map	*map_info;

	game = get_game();
	game->mlx = NULL;
	game->win = NULL;
	map_info = parse_map(map_path);
	if (!map_info)
	{
		ft_printf("Invalid map\n");
		exit_error();
	}
	game->map = *map_info;
	free(map_info);
	init_window();
	init_sprites();
	setup_hooks();
}

void	init_window(void)
{
	t_game	*game;

	game = get_game();
	game->window_width = game->map.width * SPRITE_SIZE;
	game->window_height = game->map.height * SPRITE_SIZE;
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, game->window_width,
			game->window_height, "so_long");
	game->game_over = false;
	game->vic = false;
	if (!game->mlx || !game->win)
		exit_error();
}

void	init_sprites(void)
{
	get_game()->canvas = make_sprite(NULL);
	get_game()->floor = make_sprite("assets/terrain/floor00.xpm");
	get_game()->floor2 = make_sprite("assets/terrain/floor01.xpm");
	get_game()->victory = make_sprite("assets/victory.xpm");
	get_game()->defeat = make_sprite("assets/defeat.xpm");
	init_collectible();
	init_wall();
	init_player();
	init_enemy();
	init_exit();
	init_health();
	init_mushroom();
}

void	setup_hooks(void)
{
	mlx_loop_hook(get_game()->mlx, game_loop, NULL);
	mlx_hook(get_game()->win, KeyPress, KeyPressMask, key_loop, "p");
	mlx_hook(get_game()->win, KeyRelease, KeyReleaseMask, key_loop, "r");
	mlx_hook(get_game()->win, DestroyNotify, KeyPressMask, exit_game, NULL);
}

static bool	is_map_surrounded(t_map *map)
{
	int	i;

	i = -1;
	while (++i < map->width)
		if (map->map[0][i] != '1' || map->map[map->height - 1][i] != '1')
			return (false);
	i = -1;
	while (++i < map->height)
		if (map->map[i][0] != '1' || map->map[i][map->width - 1] != '1')
			return (false);
	return (true);
}

static bool	count_entities(t_map *map, t_counter *count)
{
	int	i;
	int	j;

	count->player = 0;
	count->exit = 0;
	count->collect = 0;
	count->empty = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == 'P')
				count->player++;
			else if (map->map[i][j] == 'E')
				count->exit++;
			else if (map->map[i][j] == 'C')
				count->collect++;
			else if (map->map[i][j] == '0')
				count->empty++;
		}
	}
	return (count->player == 1 && count->exit == 1 && count->collect > 0
		&& count->empty > 0);
}

static void	flood_fill(char **map, int x, int y, int *collect)
{
	if (map[y][x] == '1' || map[y][x] == 'X')
		return ;
	if (map[y][x] == 'C')
		(*collect)--;
	map[y][x] = 'X';
	flood_fill(map, x + 1, y, collect);
	flood_fill(map, x - 1, y, collect);
	flood_fill(map, x, y + 1, collect);
	flood_fill(map, x, y - 1, collect);
}

static char	**create_temp_map(t_map *map)
{
	char	**temp_map;
	int		i;

	temp_map = malloc(sizeof(char *) * map->height);
	if (!temp_map)
		return (NULL);
	i = -1;
	while (++i < map->height)
	{
		temp_map[i] = ft_strdup(map->map[i]);
		if (!temp_map[i])
		{
			while (--i >= 0)
				free(temp_map[i]);
			free(temp_map);
			return (NULL);
		}
	}
	return (temp_map);
}

static void	find_player_pos(t_map *map, int *px, int *py)
{
	int	i;
	int	j;

	*px = -1;
	*py = -1;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == 'P')
			{
				*px = j;
				*py = i;
				return ;
			}
		}
	}
}

static int	count_collectibles(char **map, int height, int width)
{
	int	i;
	int	j;
	int	count;

	count = 0;
	i = -1;
	while (++i < height)
	{
		j = -1;
		while (++j < width)
			if (map[i][j] == 'C')
				count++;
	}
	return (count);
}

static bool	check_exit_access(char **temp_map, char **map, int height,
		int width)
{
	int	i;
	int	j;

	i = -1;
	while (++i < height)
	{
		j = -1;
		while (++j < width)
			if (map[i][j] == 'E' && temp_map[i][j] == 'X')
				return (true);
	}
	return (false);
}

bool	check_path(t_map *map)
{
	char	**temp_map;
	int		px;
	int		py;
	int		collect;
	bool	valid_path;

	temp_map = create_temp_map(map);
	if (!temp_map)
		return (false);
	find_player_pos(map, &px, &py);
	collect = count_collectibles(map->map, map->height, map->width);
	flood_fill(temp_map, px, py, &collect);
	valid_path = check_exit_access(temp_map, map->map, map->height, map->width);
	px = -1;
	while (++px < map->height)
		free(temp_map[px]);
	free(temp_map);
	return (valid_path && collect == 0);
}

static bool	validate_map(t_map *map)
{
	t_counter	count;

	if (!is_map_surrounded(map))
		return (false);
	if (!count_entities(map, &count))
		return (false);
	if (!check_path(map))
		return (false);
	return (true);
}

static t_map	*init_map_info(void)
{
	t_map	*map_info;

	map_info = malloc(sizeof(t_map));
	if (!map_info)
		return (NULL);
	map_info->height = 0;
	map_info->width = 0;
	return (map_info);
}

static bool	allocate_map_rows(t_map *map_info)
{
	map_info->map = malloc(sizeof(char *) * (map_info->height + 1));
	if (!map_info->map)
	{
		free(map_info);
		return (false);
	}
	return (true);
}

static void	count_dimensions(t_map *map_info, int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line != NULL)
	{
		if (map_info->height == 0)
			map_info->width = ft_strlen(line) - 1;
		map_info->height++;
		free(line);
		line = get_next_line(fd);
	}
}

static bool	read_map_content(t_map *map_info, int fd)
{
	char	*line;
	int		i;

	i = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		map_info->map[i] = line;
		i++;
		line = get_next_line(fd);
	}
	map_info->map[i] = NULL;
	if (!validate_map(map_info))
	{
		i = -1;
		while (++i < map_info->height)
			free(map_info->map[i]);
		free(map_info->map);
		free(map_info);
		return (false);
	}
	return (true);
}

t_map	*parse_map(char *filename)
{
	t_map	*map_info;
	int		fd;

	map_info = init_map_info();
	if (!map_info)
		return (NULL);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		free(map_info);
		return (NULL);
	}
	count_dimensions(map_info, fd);
	close(fd);
	if (!allocate_map_rows(map_info))
		return (NULL);
	fd = open(filename, O_RDONLY);
	if (!read_map_content(map_info, fd))
		return (NULL);
	close(fd);
	return (map_info);
}

static bool	check_file_extension(char *filename)
{
	char	*dot;

	dot = ft_strrchr(filename, '.');
	if (!dot || ft_strcmp(dot, ".ber") != 0)
		return (false);
	return (true);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (ft_printf("Error\nUsage: ./so_long [map.ber]\n"));
	if (!check_file_extension(argv[1]))
		return (ft_printf("Error\nFile extension not supported\n"));
	srand(time(NULL));
	init_game(argv[1]);
	mlx_loop(get_game()->mlx);
	exit_game();
	return (0);
}
