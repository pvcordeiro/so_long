/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_game_state.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 01:40:03 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 01:40:17 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

static void	update_player_invincibility(t_player *player)
{
	if (player->invincibility_frames > 0)
		player->invincibility_frames--;
}

static bool	check_enemy_hit(t_player *player, t_enemy *enemy)
{
	t_position	pos1;
	t_position	pos2;

	pos1.x = player->x + PLAYER_COLLISION_X_OFFSET;
	pos1.y = player->y + PLAYER_COLLISION_Y_OFFSET;
	pos2.x = enemy->x + ENEMY_HITBOX_X_OFFSET;
	pos2.y = enemy->y + ENEMY_HITBOX_Y_OFFSET;
	return (check_collision(pos1, pos2, PLAYER_COLLISION_WIDTH,
			PLAYER_COLLISION_HEIGHT));
}

static void	handle_enemy_hit(t_player *player)
{
	player->lives--;
	player->invincibility_frames = PLAYER_INVINCIBILITY_DURATION;
	if (player->lives <= 0)
		get_game()->game_over = true;
}

void	handle_game_state(void)
{
	t_player		*player;
	t_enemy_manager	*enemy_list;
	int				i;

	player = &get_game()->player;
	enemy_list = &get_game()->enemy_list;
	update_player_invincibility(player);
	i = -1;
	while (++i < enemy_list->count)
	{
		if (!enemy_list->enemies[i].is_dead
			&& player->invincibility_frames == 0)
		{
			if (check_enemy_hit(player, &enemy_list->enemies[i]))
			{
				handle_enemy_hit(player);
				break ;
			}
		}
	}
}
