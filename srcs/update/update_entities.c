/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_entities.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:45:48 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 01:38:24 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

static void	update_mushroom(void)
{
	t_mushroom	*mushroom;
	t_player	*player;
	t_position	pos1;
	t_position	pos2;

	mushroom = &get_game()->mushroom;
	player = &get_game()->player;
	if (!mushroom->active || mushroom->collected)
		return ;
	pos1.x = player->x;
	pos1.y = player->y;
	pos2.x = mushroom->x + HITBOX_X_OFFSET;
	pos2.y = mushroom->y + HITBOX_Y_OFFSET;
	if (check_collision(pos1, pos2, COLLECTIBLE_SIZE, COLLECTIBLE_SIZE))
	{
		if (player->lives < 3)
		{
			player->lives++;
			mushroom->collected = true;
			mushroom->active = false;
		}
	}
}

static void	update_single_collectible(int i, t_collectible *collectible,
		t_position player_pos)
{
	t_position	collect_pos;

	if (!collectible->collected[i])
	{
		collect_pos.x = collectible->x_positions[i] + HITBOX_X_OFFSET;
		collect_pos.y = collectible->y_positions[i] + HITBOX_Y_OFFSET;
		if (check_collision(player_pos, collect_pos, COLLECTIBLE_SIZE,
				COLLECTIBLE_SIZE))
		{
			collectible->collected[i] = 1;
			get_game()->collectible_count++;
		}
	}
}

static void	update_collectible(void)
{
	int				i;
	t_collectible	*collectible;
	t_player		*player;
	t_position		player_pos;

	collectible = &get_game()->collectible;
	player = &get_game()->player;
	player_pos.x = player->x;
	player_pos.y = player->y;
	i = -1;
	while (++i < collectible->count)
		update_single_collectible(i, collectible, player_pos);
	update_sprite_animation(&collectible->base);
}

void	update_entities(void)
{
	update_mushroom();
	update_collectible();
	update_enemy();
	check_attack_collision();
	update_player_position();
	update_player();
}
