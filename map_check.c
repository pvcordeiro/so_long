/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 22:58:08 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/13 23:07:13 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	check_map_borders(char **map, int width, int height)
{
	int	x;
	int	y;

	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			if (y == 0 || y == height - 1 || x == 0 || x == width - 1)
			{
				if (map[y][x] != '1')
					exit_error("Map is not surrounded by walls");
			}
			x++;
		}
		y++;
	}
}

static void	check_map_elements(char **map, int width, int height)
{
	int	x;
	int	y;
	int	player;
	int	exit;
	int	collectible;

	player = 0;
	exit = 0;
	collectible = 0;
	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			if (map[y][x] == 'P')
				player++;
			if (map[y][x] == 'E')
				exit++;
			if (map[y][x] == 'C')
				collectible++;
			x++;
		}
		y++;
	}
	if (player != 1)
		exit_error("Map must have one player");
	if (exit != 1)
		exit_error("Map must have one exit");
	if (collectible < 1)
		exit_error("Map must have at least one collectible");
}

void	check_map(char **map, int width, int height)
{
	check_map_borders(map, width, height);
	check_map_elements(map, width, height);
}

void	exit_error(char *message)
{
	ft_printf("%s\n", message);
	exit(EXIT_FAILURE);
}