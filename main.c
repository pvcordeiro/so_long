/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:14:46 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/12 14:18:38 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

t_game	*get_game(void)
{
	static t_game	game;

	return (&game);
}
int	get_pixel(t_img *data, int x, int y)
{
	
	char	*dst;

	dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
	return (*(unsigned int*)dst);
	
}

void	set_pixel(t_img *img, int x, int y, int color)
{
	char *pixel;
	
	pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)pixel = color;
}

int	exit_game(void)
{
	mlx_destroy_window(get_game()->mlx, get_game()->win);
	mlx_destroy_display(get_game()->mlx);
	free(get_game()->mlx);
	exit(EXIT_SUCCESS);
}


// void	create_image(void *mlx_ptr, void *win_ptr, void *img_ptr, int x, int y)
// {
// 	mlx_clear_window(mlx_ptr, win_ptr);
// 	mlx_put_image_to_window(mlx_ptr, win_ptr, img_ptr, x, y);
// }

void	draw_player(void)
{
	t_img *player;

	player = &get_game()->player;
	mlx_put_image_to_window(get_game()->mlx, get_game()->win, player->img, player->x, player->y);
}

void	draw_image(t_img *img, int x, int y)
{
	int	sx;
	int	sy;
	t_img *canvas;

	canvas = &get_game()->canvas;
	sy = 0;
	while(sy < img->height)
	{
		sx = 0;
		while(sx < img->width)
		{
			if (!(sx + x > WINDOW_WIDTH || sy + y > WINDOW_HEIGHT || sx + x < 0 || sy + y < 0))
				set_pixel(canvas, x + sx, y + sy, get_pixel(img, sx, sy));
			sx++;
		}
		sy++;
	}
}

void	draw_background(void)
{
	int	x;
	int	y;
	t_img *canvas;

	canvas = &get_game()->canvas;
	y = 0;
	while(y < WINDOW_HEIGHT)
	{
		x = 0;
		while(x < WINDOW_WIDTH)
		{
			set_pixel(canvas, x, y, 0x00FF0000);
			x++;
		}
		y++;
	}
	draw_image(&get_game()->floor, 0, 0);
	draw_image(&get_game()->player, get_game()->player.x, get_game()->player.y);
	mlx_put_image_to_window(get_game()->mlx, get_game()->win, canvas->img, 0, 0);
}

void	draw_floor(void)
{
	t_img *floor;

	floor = &get_game()->floor;
	mlx_put_image_to_window(get_game()->mlx, get_game()->win, floor->img, floor->x, floor->y);
}

int	key_loop(int key)
{
	if (key == XK_w)
		get_game()->player.y -= 10;
	if (key == XK_a)
		get_game()->player.x -= 10;
	if (key == XK_s)
		get_game()->player.y += 10;
	if (key == XK_d)
		get_game()->player.x += 10;
	if (key == XK_Escape)
		exit_game();
	draw_background();
	return (0);
}

t_img	make_sprite(char *path)
{
	t_img	sprite;
	
	sprite.img = mlx_xpm_file_to_image(get_game()->mlx, path, &sprite.width, &sprite.height);
	sprite.x = 0;
	sprite.y = 0;
	sprite.addr = mlx_get_data_addr(sprite.img, &sprite.bpp, &sprite.line_len, &sprite.endian);
	return (sprite);
}

void init_canvas(t_img *canvas)
{
	canvas->img = mlx_new_image(get_game()->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	canvas->addr = mlx_get_data_addr(canvas->img, &canvas->bpp, &canvas->line_len, &canvas->endian);
}

int	main(void)
{

	get_game()->mlx = mlx_init();
	get_game()->win = mlx_new_window(get_game()->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "so_long");
	init_canvas(&get_game()->canvas);
	get_game()->player = make_sprite("textures/player.xpm");
	get_game()->floor = make_sprite("textures/floor_grey.xpm");
	draw_background();
	mlx_key_hook(get_game()->win, key_loop, NULL);
	mlx_hook(get_game()->win, DestroyNotify, KeyPressMask, exit_game, NULL);
	mlx_loop(get_game()->mlx);
	free(get_game()->mlx);
}
