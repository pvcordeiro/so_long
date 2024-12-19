/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:44:48 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:56:37 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

int	check_collision(t_position pos1, t_position pos2, int width,
		int height)
{
	return (pos1.x < (pos2.x + width) && (pos1.x + width) > pos2.x
		&& pos1.y < (pos2.y + height) && (pos1.y + height) > pos2.y);
}

int	check_wall_collisions(int x, int y, int width, int height)
{
	t_wall_manager	*wall;
	int				i;
	t_position		pos1;
	t_position		pos2;

	wall = &get_game()->wall;
	pos1.x = x;
	pos1.y = y;
	i = -1;
	while (++i < wall->count)
	{
		pos2.x = wall->x_positions[i];
		pos2.y = wall->y_positions[i];
		if (check_collision(pos1, pos2, width, height))
			return (1);
	}
	return (0);
}

int	check_enemy_collisions(int x, int y, int current_enemy)
{
	t_enemy_manager	*enemy_list;
	int				i;
	t_position		pos1;
	t_position		pos2;

	enemy_list = &get_game()->enemy_list;
	i = -1;
	pos1.x = x + ENEMY_HITBOX_X_OFFSET;
	pos1.y = y + ENEMY_HITBOX_Y_OFFSET;
	while (++i < enemy_list->count)
	{
		if (i != current_enemy && !enemy_list->enemies[i].is_dead)
		{
			pos2.x = enemy_list->enemies[i].x + ENEMY_HITBOX_X_OFFSET;
			pos2.y = enemy_list->enemies[i].y + ENEMY_HITBOX_Y_OFFSET;
			if (check_collision(pos1, pos2, ENEMY_COLLISION_WIDTH,
					ENEMY_COLLISION_HEIGHT))
				return (1);
		}
	}
	return (0);
}

void	handle_entity_collision(int *x, int *y, int prev_x, int prev_y)
{
	if (check_wall_collisions(*x + PLAYER_COLLISION_X_OFFSET, *y
			+ PLAYER_COLLISION_Y_OFFSET, PLAYER_COLLISION_WIDTH,
			PLAYER_COLLISION_HEIGHT))
	{
		*x = prev_x;
		*y = prev_y;
	}
}
