/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 22:58:08 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/14 03:31:05 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int is_valid_char(char c)
{
    return (c == '1' || c == '0' || c == 'P' || c == 'E' || c == 'C' || c == 'F');
}

static int check_file_extension(char *filename)
{
    char *dot = ft_strrchr(filename, '.');
    if (!dot || ft_strcmp(dot, ".ber") != 0)
        return (0);
    return (1);
}

static int flood_fill(char **map, int x, int y, t_map *map_info)
{
    if (x < 0 || y < 0 || x >= map_info->width || y >= map_info->height || 
        map[y][x] == '1' || map[y][x] == 'V')
        return (0);

    if (map[y][x] == 'C')
        map_info->collectibles_reachable++;
    if (map[y][x] == 'E')
        map_info->exit_reachable = 1;

    map[y][x] = 'V'; // Mark as visited

    flood_fill(map, x + 1, y, map_info);
    flood_fill(map, x - 1, y, map_info);
    flood_fill(map, x, y + 1, map_info);
    flood_fill(map, x, y - 1, map_info);

    return (1);
}

static int check_path(t_map *map_info)
{
    char **temp_map;
    int i, start_x = -1, start_y = -1;

    // Create temporary map for flood fill
    temp_map = malloc(sizeof(char *) * map_info->height);
    for (i = 0; i < map_info->height; i++)
        temp_map[i] = ft_strdup(map_info->map[i]);

    // Find player position
    for (i = 0; i < map_info->height; i++) {
        char *p = ft_strchr(temp_map[i], 'P');
        if (p) {
            start_x = p - temp_map[i];
            start_y = i;
            break;
        }
    }

    map_info->collectibles_reachable = 0;
    map_info->exit_reachable = 0;

    flood_fill(temp_map, start_x, start_y, map_info);

    // Free temporary map
    for (i = 0; i < map_info->height; i++)
        free(temp_map[i]);
    free(temp_map);

    return (map_info->collectibles_reachable == map_info->collectibles &&
            map_info->exit_reachable);
}

static int validate_map(t_map *map_info)
{
    int i, j;
    int player_count = 0;
    int exit_count = 0;

    // Check if map is rectangular and surrounded by walls
    for (i = 0; i < map_info->height; i++) {
        if ((int)ft_strlen(map_info->map[i]) != map_info->width)
            return (0);
        if (map_info->map[i][0] != '1' || map_info->map[i][map_info->width - 1] != '1')
            return (0);
    }
    for (j = 0; j < map_info->width; j++) {
        if (map_info->map[0][j] != '1' || map_info->map[map_info->height - 1][j] != '1')
            return (0);
    }

    // Check valid characters and count elements
    for (i = 0; i < map_info->height; i++) {
        for (j = 0; j < map_info->width; j++) {
            if (!is_valid_char(map_info->map[i][j]))
                return (0);
            if (map_info->map[i][j] == 'P')
                player_count++;
            else if (map_info->map[i][j] == 'E')
                exit_count++;
            else if (map_info->map[i][j] == 'C')
                map_info->collectibles++;
        }
    }

    return (player_count == 1 && exit_count == 1 && map_info->collectibles > 0);
}

t_map *parse_map(char *filename)
{
    t_map *map_info;
    char *line;
    int fd, i;

    if (!check_file_extension(filename))
        return (NULL);

    map_info = malloc(sizeof(t_map));
    if (!map_info)
        return (NULL);

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        free(map_info);
        return (NULL);
    }

    // Count lines and get width
    map_info->height = 0;
    map_info->width = 0;
    while ((line = get_next_line(fd)))
    {
        if (map_info->height == 0)
            map_info->width = ft_strlen(line);
        map_info->height++;
        free(line);
    }
    close(fd);

    if (map_info->height == 0 || map_info->width == 0)
    {
        free(map_info);
        return (NULL);
    }

    // Allocate and read map
    map_info->map = malloc(sizeof(char *) * (map_info->height + 1));
    if (!map_info->map)
    {
        free(map_info);
        return (NULL);
    }

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        free(map_info->map);
        free(map_info);
        return (NULL);
    }

    i = 0;
    while ((line = get_next_line(fd)))
        map_info->map[i++] = line;
    map_info->map[i] = NULL;
    close(fd);

    map_info->collectibles = 0;
    map_info->collectibles_reachable = 0;
    map_info->exit_reachable = 0;

    if (!validate_map(map_info) || !check_path(map_info))
    {
        for (i = 0; map_info->map[i]; i++)
            free(map_info->map[i]);
        free(map_info->map);
        free(map_info);
        return (NULL);
    }

    return (map_info);
}