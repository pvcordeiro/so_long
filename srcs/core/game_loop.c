/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:47:21 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 01:00:50 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

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