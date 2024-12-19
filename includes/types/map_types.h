/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_types.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 03:09:58 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:29:39 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_TYPES_H
# define MAP_TYPES_H
# include "sprite_types.h"

# define WALL_ANIMATION_SPEED 100

typedef struct s_map_entity_counts
{
	int					player;
	int					exit;
	int					collect;
	int					empty;
}						t_map_entity_counts;

typedef struct s_map
{
	char				**map;
	int					width;
	int					height;
}						t_map;

typedef struct s_wall_manager
{
	t_animated_sprite	base;
	int					*x_positions;
	int					*y_positions;
	int					count;
}						t_wall_manager;

typedef struct s_exit
{
	t_sprite			sprite;
	int					x;
	int					y;
}						t_exit;

#endif