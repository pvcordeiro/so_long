/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_frame.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:45:35 by paude-so          #+#    #+#             */
/*   Updated: 2025/01/02 14:52:48 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

static void	draw_entity_layer(void)
{
	draw_mushroom();
	draw_collectibles();
	draw_enemy();
}

static void	draw_player_and_exit(void)
{
	if (get_game()->player.y < get_game()->exit.y)
	{
		draw_player();
		draw_exit_full();
	}
	else
	{
		draw_exit_part(false);
		draw_player();
		draw_exit_part(true);
	}
}

static void	draw_ui_layer(void)
{
	draw_helper_message();
	draw_ui_banners();
	draw_sprint_icon();
	draw_health();
	draw_collectible_counter();
}

void	draw_frame(void)
{
	t_game_state	*game;

	game = get_game();
	draw_floor();
	draw_walls();
	if (!game->vic && !game->game_over)
	{
		draw_entity_layer();
		draw_player_and_exit();
		draw_ui_layer();
	}
	draw_end_game_screen();
}
