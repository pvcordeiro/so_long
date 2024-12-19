/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 03:10:18 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:51:27 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_H
# define PLAYER_H

# include "../types/game_types.h"
# include "../types/player_types.h"

void	init_player(void);
void	update_player(void);
void	update_player_position(void);
void	handle_movement(t_player *player, int movement_speed, int *prev_x,
			int *prev_y);
void	handle_movement_keys(t_game_state *game, int key, char *action);
void	handle_sprint_key(t_game_state *game, char *action);
void	handle_sprint_cooldown(t_player *player);
void	handle_sprint(t_player *player, int *movement_speed);
void	handle_attack_key(t_player *player);
void	draw_player(void);
void	load_player_animations(t_player *player);

#endif