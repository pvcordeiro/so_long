/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endgame.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:21:31 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 02:22:02 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	victory_check(void)
{
	t_position	pos1;
	t_position	pos2;

	pos1.x = get_game()->player.x;
	pos1.y = get_game()->player.y;
	pos2.x = get_game()->exit.x;
	pos2.y = get_game()->exit.y;
	if (get_game()->collectible_count == get_game()->collectible.count
		&& check_collision(pos1, pos2, 20, 20))
	{
		get_game()->vic = true;
	}
}

void	draw_end_game_screen(void)
{
	t_game_state	*game;
	int				x;
	int				y;

	game = get_game();
	if (game->vic || game->game_over)
	{
		x = (game->window_width - 400) / 2;
		y = (game->window_height - 400) / 2;
		if (game->vic)
			draw_sprite(&game->victory, &game->canvas, x, y);
		else
			draw_sprite(&game->defeat, &game->canvas, x, y);
	}
}
