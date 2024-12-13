/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 22:58:08 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/13 23:10:51 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int check_file_extension(char *filename)
{
    int i;

    i = 0;
    while (filename[i])
        i++;
    if (i < 4)
        return (0);
    if (filename[i - 4] != '.' || filename[i - 3] != 'b' ||
        filename[i - 2] != 'e' || filename[i - 1] != 'r')
        return (0);
    return (1);
}

static int count_map_rows(char *filename)
{
    int     fd;
    int     rows;
    char    *line;

    rows = 0;
    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (-1);
    while ((line = get_next_line(fd)))
    {
        rows++;
        free(line);
    }
    close(fd);
    return (rows);
}

static int validate_map_chars(char **map)
{
    int i;
    int j;
    int player;
    int exit;

    player = 0;
    exit = 0;
    i = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            if (map[i][j] != '1' && map[i][j] != '0' && map[i][j] != 'P' &&
                map[i][j] != 'E' && map[i][j] != 'C' && map[i][j] != 'e')
                return (0);
            if (map[i][j] == 'P')
                player++;
            if (map[i][j] == 'E')
                exit++;
            j++;
        }
        i++;
    }
    return (player == 1 && exit == 1);
}

static int validate_map_walls(char **map)
{
    int i;
    int j;
    int height;
    int width;

    height = 0;
    while (map[height])
        height++;
    width = ft_strlen(map[0]);

    i = 0;
    while (i < height)
    {
        j = 0;
        while (map[i][j])
        {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
                if (map[i][j] != '1')
                    return (0);
            j++;
        }
        if (j != width)
            return (0);
        i++;
    }
    return (1);
}

char **read_map(char *filename, int *height)
{
    char    **map;
    char    *line;
    int     fd;
    int     i;

    if (!check_file_extension(filename))
        return (NULL);
    
    *height = count_map_rows(filename);
    if (*height <= 0)
        return (NULL);

    map = (char **)malloc(sizeof(char *) * (*height + 1));
    if (!map)
        return (NULL);

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        free(map);
        return (NULL);
    }

    i = 0;
    while ((line = get_next_line(fd)))
    {
        map[i] = line;
        i++;
    }
    map[i] = NULL;
    close(fd);

    if (!validate_map_chars(map) || !validate_map_walls(map))
    {
        i = 0;
        while (map[i])
            free(map[i++]);
        free(map);
        return (NULL);
    }

    return (map);
}

