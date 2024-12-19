/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mushroom.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:44:32 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 02:45:59 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

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