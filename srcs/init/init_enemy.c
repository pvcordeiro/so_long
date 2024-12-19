/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_enemy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 01:24:08 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 01:24:30 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

static bool	allocate_enemy_array(t_enemy_manager *enemy_list, int count)
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

static void	setup_enemies(t_enemy_manager *enemy_list, t_map *map)
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
	t_enemy_manager	*enemy_list;
	t_map			*map;
	int				count;

	enemy_list = &get_game()->enemy_list;
	map = &get_game()->map;
	count = count_map_char(map->map, map->height, map->width, 'X');
	if (!allocate_enemy_array(enemy_list, count))
		exit_error(ERR_MEMORY);
	setup_enemies(enemy_list, map);
}
