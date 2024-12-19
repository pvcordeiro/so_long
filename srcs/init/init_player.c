/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:46:33 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:56:21 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

static void	init_player_position(t_player *player, t_map *map)
{
	init_entity_position(map, 'P', &player->x, &player->y);
	player->state = IDLE_RIGHT;
	player->last_direction = MOVE_RIGHT;
}

static void	init_player_stats(t_player *player)
{
	player->lives = 3;
	player->invincibility_frames = 0;
	player->is_visible = 1;
}

static void	init_player_combat(t_player *player)
{
	player->attack_cooldown = 0;
	player->max_attack_cooldown = ATTACK_COOLDOWN;
	player->is_attacking = false;
	player->attack_frame = 0;
}

static void	init_player_movement(t_player *player)
{
	player->is_sprinting = false;
	player->sprint_duration = 0;
	player->sprint_cooldown = 0;
	player->can_sprint = true;
}

void	init_player(void)
{
	t_player	*player;
	t_map		*map;

	player = &get_game()->player;
	map = &get_game()->map;
	init_player_position(player, map);
	init_player_stats(player);
	init_player_combat(player);
	init_player_movement(player);
	load_player_animations(player);
}
