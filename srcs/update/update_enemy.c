/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_enemy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:45:59 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 01:33:58 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

static void	update_enemy_direction(t_enemy *enemy)
{
	if (rand() % 2 == 0)
	{
		enemy->direction = (rand() % 2) * 2 - 1;
		enemy->y_direction = 0;
	}
	else
	{
		enemy->direction = 0;
		enemy->y_direction = (rand() % 2) * 2 - 1;
	}
	enemy->move_counter = 0;
}

static void	update_enemy_movement(t_enemy *enemy)
{
	int	prev_x;
	int	prev_y;

	enemy->move_counter++;
	if (enemy->move_counter >= ENEMY_DIRECTION_CHANGE_DELAY)
		update_enemy_direction(enemy);
	prev_x = enemy->x;
	prev_y = enemy->y;
	enemy->x += enemy->direction * ENEMY_SPEED;
	enemy->y += enemy->y_direction * ENEMY_SPEED;
	handle_enemy_collision(enemy, prev_x, prev_y);
}

static void	update_enemy_state(t_enemy *enemy)
{
	t_animated_sprite	*current_anim;

	if (enemy->is_dead)
		return ;
	if (enemy->invincibility_frames > 0)
		enemy->invincibility_frames--;
	update_enemy_movement(enemy);
	if (enemy->direction != 0)
	{
		if (enemy->direction > 0)
			enemy->state = MOVE_RIGHT;
		else
			enemy->state = MOVE_LEFT;
	}
	if (enemy->state == MOVE_RIGHT)
		current_anim = &enemy->move_right;
	else
		current_anim = &enemy->move_left;
	update_sprite_animation(current_anim);
}

void	update_enemy(void)
{
	t_enemy_manager	*enemy_list;
	int				i;

	enemy_list = &get_game()->enemy_list;
	i = -1;
	while (++i < enemy_list->count)
		update_enemy_state(&enemy_list->enemies[i]);
}