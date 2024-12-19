/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:26:34 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 02:30:10 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

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
