/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:14:46 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 02:30:15 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}



void	draw_text_layer(void)
{
	t_game_state	*game;
	char			*count_str;

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



void	update_game_state(void)
{
	if (get_game()->vic || get_game()->game_over)
		return ;
	handle_game_state();
	update_entities();
	victory_check();
}







void	init_animated_sprite(t_animated_sprite *anim, int frame_count,
		int speed)
{
	anim->current_frame = 0;
	anim->anim_counter = 0;
	anim->anim_speed = speed;
	anim->frame_count = frame_count;
}

void	update_sprite_animation(t_animated_sprite *anim)
{
	anim->anim_counter++;
	if (anim->anim_counter >= anim->anim_speed)
	{
		anim->anim_counter = 0;
		anim->current_frame = (anim->current_frame + 1) % anim->frame_count;
	}
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

// void	handle_enemy_movement(t_enemy *enemy)
// {
// 	int	prev_x;
// 	int	prev_y;

// 	prev_x = enemy->x;
// 	prev_y = enemy->y;
// 	enemy->x += enemy->direction * ENEMY_SPEED;
// 	enemy->y += enemy->y_direction * ENEMY_SPEED;
// 	if (check_wall_collisions(enemy->x + ENEMY_HITBOX_X_OFFSET, enemy->y
// 			+ ENEMY_HITBOX_Y_OFFSET, ENEMY_COLLISION_WIDTH,
// 			ENEMY_COLLISION_HEIGHT))
// 	{
// 		enemy->x = prev_x;
// 		enemy->y = prev_y;
// 		enemy->direction *= -1;
// 		enemy->y_direction *= -1;
// 	}
// }



static bool	is_enemy_visible(t_enemy *enemy)
{
	if (enemy->invincibility_frames > 0)
	{
		enemy->is_visible = !enemy->is_visible;
		return (enemy->is_visible);
	}
	return (true);
}

static t_animated_sprite	*get_enemy_animation(t_enemy *enemy)
{
	if (enemy->state == MOVE_RIGHT)
		return (&enemy->move_right);
	return (&enemy->move_left);
}

static void	draw_single_enemy(t_enemy *enemy)
{
	t_animated_sprite	*current_anim;

	if (!enemy->is_dead && is_enemy_visible(enemy))
	{
		current_anim = get_enemy_animation(enemy);
		current_anim->x = enemy->x;
		current_anim->y = enemy->y;
		draw_animated_sprite(current_anim, &get_game()->canvas);
	}
}

void	draw_enemy(void)
{
	t_enemy_manager	*enemy_list;
	int				i;

	enemy_list = &get_game()->enemy_list;
	i = -1;
	while (++i < enemy_list->count)
		draw_single_enemy(&enemy_list->enemies[i]);
}

void	init_ui_elements(void)
{
	t_ui_elements	*health;

	health = &get_game()->health;
	health->health1 = create_sprite("assets/health/health1.xpm");
	health->health2 = create_sprite("assets/health/health2.xpm");
	health->health3 = create_sprite("assets/health/health3.xpm");
	health->sprint = create_sprite("assets/misc/sprint.xpm");
	health->banner = create_sprite("assets/misc/banner.xpm");
	health->message = create_sprite("assets/misc/helper_message.xpm");
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
	mushroom->sprite = create_sprite("assets/misc/mushroom.xpm");
	mushroom->active = true;
	mushroom->collected = false;
	valid_positions = count_valid_positions(map);
	find_random_position(mushroom, map, rand() % valid_positions);
}

void	draw_mushroom(void)
{
	t_mushroom	*mushroom;

	mushroom = &get_game()->mushroom;
	if (mushroom->active && !mushroom->collected)
	{
		draw_sprite(&mushroom->sprite, &get_game()->canvas, mushroom->x,
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



void	load_player_animations(t_player *player)
{
	load_animation_sprites(&player->idle_right,
		"assets/player/idle_right/player_idle_right00.xpm",
		"assets/player/idle_right/player_idle_right01.xpm",
		PLAYER_IDLE_FRAME_DELAY);
	load_animation_sprites(&player->idle_left,
		"assets/player/idle_left/player_idle_left00.xpm",
		"assets/player/idle_left/player_idle_left01.xpm",
		PLAYER_IDLE_FRAME_DELAY);
	load_animation_sprites(&player->move_right,
		"assets/player/move_right/player_move_right00.xpm",
		"assets/player/move_right/player_move_right01.xpm",
		PLAYER_IDLE_FRAME_DELAY);
	load_animation_sprites(&player->move_left,
		"assets/player/move_left/player_move_left00.xpm",
		"assets/player/move_left/player_move_left01.xpm",
		PLAYER_IDLE_FRAME_DELAY);
	load_animation_sprites(&player->attack_right,
		"assets/player/attack_right/player_attack_right00.xpm",
		"assets/player/attack_right/player_attack_right01.xpm",
		PLAYER_IDLE_FRAME_DELAY);
	load_animation_sprites(&player->attack_left,
		"assets/player/attack_left/player_attack_left00.xpm",
		"assets/player/attack_left/player_attack_left01.xpm",
		PLAYER_IDLE_FRAME_DELAY);
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

static t_animated_sprite	*get_player_animation(t_player *player)
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
	t_player			*player;
	t_animated_sprite	*current_anim;

	player = &get_game()->player;
	if (!is_player_visible(player))
		return ;
	current_anim = get_player_animation(player);
	current_anim->x = player->x;
	current_anim->y = player->y;
	draw_animated_sprite(current_anim, &get_game()->canvas);
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
		return (ft_printf("Error\nFile must have .ber extension\n"));
	srand(time(NULL));
	init_game_state(argv[1]);
	mlx_loop(get_game()->mlx);
	exit_game_state();
	return (0);
}
