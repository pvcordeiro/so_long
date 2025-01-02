/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:22:50 by paude-so          #+#    #+#             */
/*   Updated: 2025/01/02 12:23:03 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

char	**create_temp_map(t_map *map)
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

static bool	count_entities(t_map *map, t_map_entity_counts *count)
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

static bool	check_rectangular_map(t_map *map)
{
	int		i;
	size_t	current_line_len;

	i = -1;
	while (++i < map->height)
	{
		current_line_len = ft_strlen(map->map[i]);
		if (map->map[i][current_line_len - 1] == '\n')
			current_line_len--;
		if (current_line_len != (size_t)map->width)
			return (false);
	}
	return (true);
}

t_error	validate_map(t_map *map)
{
	t_map_entity_counts	count;

	if (!check_rectangular_map(map))
		return (ERR_MAP_NOT_RECTANGULAR);
	if (!is_map_surrounded(map))
		return (ERR_WALLS);
	if (!count_entities(map, &count))
	{
		if (count.player == 0)
			return (ERR_NO_PLAYER);
		if (count.player > 1)
			return (ERR_MULTIPLE_PLAYERS);
		if (count.exit == 0)
			return (ERR_NO_EXIT);
		if (count.exit > 1)
			return (ERR_MULTIPLE_EXITS);
		if (count.collect == 0)
			return (ERR_NO_COLLECTIBLES);
	}
	if (!check_path(map))
		return (ERR_INVALID_PATH);
	return (ERR_NONE);
}
