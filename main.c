/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:14:46 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/11 21:32:02 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

t_game	*get_game(void)
{
	static t_game	game;

	return (&game);
}
unsigned int	*get_pixel_pointer(t_img *img, int x, int y)
{
	return ((unsigned int *)img->addr
		+ (y * img->line_len + x * (img->bpp / 8)));
}

int	exit_game(void)
{
	mlx_destroy_window(get_game()->mlx, get_game()->win);
	mlx_destroy_display(get_game()->mlx);
	free(get_game()->mlx);
	exit(EXIT_SUCCESS);
}


int	key_loop(int key)
{
	if (key == XK_w)
		get_game()->player.y -= 5;
	if (key == XK_a)
		get_game()->player.x -= 5;
	if (key == XK_s)
		get_game()->player.y += 5;
	if (key == XK_d)
		get_game()->player.x += 5;
	if (key == XK_Escape)
		exit_game();
	mlx_clear_window(get_game()->mlx, get_game()->win);
	mlx_put_image_to_window(get_game()->mlx, get_game()->win, get_game()->player.img, get_game()->player.x, get_game()->player.y);
	return (0);
}

int	main(void)
{
	int		width;
	int		height;
	t_img	pl;

	get_game()->mlx = mlx_init();
	get_game()->win = mlx_new_window(get_game()->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "so_long");
	pl.img = mlx_xpm_file_to_image(get_game()->mlx, "textures/player.xpm", &width, &height);
	pl.x = 0;
	pl.y = 0;
	get_game()->player = pl;
	mlx_key_hook(get_game()->win, key_loop, NULL);
	mlx_hook(get_game()->win, DestroyNotify, KeyPressMask, exit_game, NULL);
	mlx_put_image_to_window(get_game()->mlx, get_game()->win, get_game()->player.img, get_game()->player.x, get_game()->player.y);
	mlx_loop(get_game()->mlx);
	free(get_game()->mlx);
}
