/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:47:21 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 19:55:30 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

static void	update_game_state(void)
{
	if (get_game()->vic || get_game()->game_over)
		return ;
	handle_game_state();
	update_entities();
	victory_check();
}

static void	draw_text_layer(void)
{
	t_game_state	*game;
	char			*count_str;

	game = get_game();
	if (game->vic || game->game_over)
		return ;
	count_str = ft_itoa(game->move_count);
	mlx_string_put(game->mlx, game->win, 25, game->window_height - 45, 0x000000,
		"MOVES");
	mlx_string_put(game->mlx, game->win, 33, game->window_height - 30, 0x000000,
		count_str);
	free(count_str);
	count_str = ft_itoa(game->collectible_count);
	mlx_string_put(game->mlx, game->win, game->window_width - 34, 45, 0x000000,
		count_str);
	free(count_str);
}

static void	fps_cap(void)
{
	static struct timeval	last_frame = {0, 0};
	struct timeval			current_time;
	long					elapsed_us;

	gettimeofday(&current_time, NULL);
	elapsed_us = (current_time.tv_sec - last_frame.tv_sec) * 1000000
		+ (current_time.tv_usec - last_frame.tv_usec);
	if (elapsed_us < FRAME_TIME_MS)
		usleep(FRAME_TIME_MS - elapsed_us);
	last_frame = current_time;
}

int	game_loop(void)
{
	fps_cap();
	update_game_state();
	draw_frame();
	mlx_put_image_to_window(get_game()->mlx, get_game()->win,
		get_game()->canvas.img, 0, 0);
	draw_text_layer();
	return (0);
}

void	init_ui_elements(void)
{
	t_ui_elements	*health;

	health = &get_game()->ui;
	health->health1 = create_sprite("assets/health/health1.xpm");
	health->health2 = create_sprite("assets/health/health2.xpm");
	health->health3 = create_sprite("assets/health/health3.xpm");
	health->sprint = create_sprite("assets/misc/sprint.xpm");
	health->banner = create_sprite("assets/misc/banner.xpm");
	health->message = create_sprite("assets/misc/helper_message.xpm");
}
