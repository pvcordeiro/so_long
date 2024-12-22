/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:46:18 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/22 14:12:03 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

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

void	init_exit(void)
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
	exit->sprite = create_sprite("assets/misc/exit.xpm");
}

void	init_wall_manager(void)
{
	t_wall_manager	*wall;
	t_map			*map;
	int				count;
	int				i;

	wall = &get_game()->wall;
	map = &get_game()->map;
	wall->count = count_map_char(map->map, map->height, map->width, '1');
	wall->x_positions = malloc(sizeof(int) * wall->count);
	wall->y_positions = malloc(sizeof(int) * wall->count);
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
