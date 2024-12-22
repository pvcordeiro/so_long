/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   attack_collision.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 01:57:00 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/22 14:33:23 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

static bool	is_valid_attack_target(t_player *player, t_enemy *enemy)
{
	return (!enemy->is_dead && player->is_attacking
		&& enemy->invincibility_frames == 0 && ((player->state == ATTACK_RIGHT
				&& enemy->x > player->x) || (player->state == ATTACK_LEFT
				&& enemy->x < player->x)));
}

static void	apply_damage_to_enemy(t_enemy *enemy, t_player *player)
{
	enemy->lives--;
	enemy->invincibility_frames = ENEMY_INVINCIBILITY_DURATION;
	if (player->x < enemy->x)
	{
		enemy->direction = 1;
		enemy->state = MOVE_RIGHT;
	}
	else
	{
		enemy->direction = -1;
		enemy->state = MOVE_LEFT;
	}
	if (enemy->lives <= 0)
		enemy->is_dead = true;
}

void	check_attack_collision(void)
{
	t_player		*player;
	t_enemy_manager	*enemy_list;
	int				i;
	t_position		pos1;
	t_position		pos2;

	player = &get_game()->player;
	enemy_list = &get_game()->enemy_list;
	pos1.x = player->x;
	pos1.y = player->y + ATTACK_COLLISION_Y_OFFSET;
	i = -1;
	while (++i < enemy_list->count)
	{
		if (is_valid_attack_target(player, &enemy_list->enemies[i]))
		{
			pos2.x = enemy_list->enemies[i].x + ENEMY_HITBOX_X_OFFSET;
			pos2.y = enemy_list->enemies[i].y + ENEMY_HITBOX_Y_OFFSET;
			if (check_collision(pos1, pos2, ATTACK_RANGE,
					ENEMY_COLLISION_HEIGHT))
			{
				apply_damage_to_enemy(&enemy_list->enemies[i], player);
			}
		}
	}
}
