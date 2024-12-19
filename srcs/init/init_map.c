/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:46:39 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 19:48:29 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

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
	free(line);
}

static bool	read_map_content(t_map *map_info, int fd)
{
	char	*line;
	int		i;
	t_error	error;

	i = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		map_info->map[i] = line;
		i++;
		line = get_next_line(fd);
	}
	free(line);
	map_info->map[i] = NULL;
	error = validate_map(map_info);
	if (error != ERR_NONE)
	{
		i = -1;
		while (++i < map_info->height)
			free(map_info->map[i]);
		free(map_info->map);
		free(map_info);
		exit_error(error);
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
