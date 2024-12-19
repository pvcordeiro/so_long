/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_frame.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:45:35 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:57:39 by paude-so         ###   ########.fr       */
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
		draw_exit_bottom();
		draw_player();
		draw_exit_top();
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

static void	draw_background_layer(void)
{
	draw_floor();
	draw_walls();
	update_sprite_animation(&get_game()->wall.base);
	draw_animated_sprite(&get_game()->wall.base, &get_game()->canvas);
}

void	draw_frame(void)
{
	t_game_state	*game;

	game = get_game();
	draw_background_layer();
	if (!game->vic && !game->game_over)
	{
		draw_entity_layer();
		draw_player_and_exit();
		draw_ui_layer();
	}
	draw_end_game_screen();
}
