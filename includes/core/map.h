/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 03:10:12 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:36:57 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H
# include "../types/map_types.h"

t_error	validate_map(t_map *map);
bool	check_path(t_map *map);
char	**create_temp_map(t_map *map);
t_map	*parse_map(char *filename);
void	init_wall_manager(void);
void	draw_walls(void);
void	draw_floor(void);
int		check_wall_collisions(int x, int y, int width, int height);
void	init_entity_position(t_map *map, char c, int *x, int *y);
int		count_map_char(char **map, int height, int width, char c);

#endif