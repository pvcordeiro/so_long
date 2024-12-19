/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:03:26 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 02:06:13 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	handle_movement_keys(t_game_state *game, int key, char *action)
{
	if (key == XK_w)
		game->move_up = (*action == 'p');
	if (key == XK_a)
		game->move_left = (*action == 'p');
	if (key == XK_s)
		game->move_down = (*action == 'p');
	if (key == XK_d)
		game->move_right = (*action == 'p');
}

void	handle_movement(t_player *player, int movement_speed,
		int *prev_x, int *prev_y)
{
	if (get_game()->move_left)
		player->x -= movement_speed;
	if (get_game()->move_right)
		player->x += movement_speed;
	handle_entity_collision(&player->x, &player->y, *prev_x, *prev_y);
	if (get_game()->move_up)
		player->y -= movement_speed;
	if (get_game()->move_down)
		player->y += movement_speed;
	handle_entity_collision(&player->x, &player->y, *prev_x, *prev_y);
}

void	handle_sprint(t_player *player, int *movement_speed)
{
	if (player->sprint_duration < SPRINT_DURATION)
	{
		*movement_speed *= SPRINT_MULTIPLIER;
		player->sprint_duration++;
	}
	else
	{
		player->is_sprinting = false;
		player->can_sprint = false;
		player->sprint_duration = 0;
	}
}

void	handle_sprint_cooldown(t_player *player)
{
	if (!player->can_sprint)
	{
		player->sprint_cooldown++;
		if (player->sprint_cooldown >= SPRINT_COOLDOWN)
		{
			player->can_sprint = true;
			player->sprint_cooldown = 0;
		}
	}
}

void	handle_sprint_key(t_game_state *game, char *action)
{
	if (*action == 'p' && game->player.can_sprint)
		game->player.is_sprinting = true;
	else if (*action == 'r')
		game->player.is_sprinting = false;
}
