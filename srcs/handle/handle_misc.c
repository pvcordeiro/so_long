/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_misc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:08:22 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 02:10:00 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	handle_enemy_collision(t_enemy *enemy, int prev_x, int prev_y)
{
	if (check_wall_collisions(enemy->x + ENEMY_HITBOX_X_OFFSET, enemy->y
			+ ENEMY_HITBOX_Y_OFFSET, ENEMY_COLLISION_WIDTH,
			ENEMY_COLLISION_HEIGHT) || check_enemy_collisions(enemy->x,
			enemy->y, enemy - get_game()->enemy_list.enemies))
	{
		enemy->x = prev_x;
		enemy->y = prev_y;
		enemy->direction *= -1;
		enemy->y_direction *= -1;
	}
}

void	handle_attack_key(t_player *player)
{
	if (!player->is_attacking && player->attack_cooldown == 0)
	{
		player->is_attacking = true;
		player->attack_frame = 0;
		if (player->state == IDLE_RIGHT || player->state == MOVE_RIGHT)
		{
			player->state = ATTACK_RIGHT;
			player->attack_right.current_frame = 0;
		}
		else
		{
			player->state = ATTACK_LEFT;
			player->attack_left.current_frame = 0;
		}
	}
}