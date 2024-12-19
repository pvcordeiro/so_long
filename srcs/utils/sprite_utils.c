/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:44:36 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 02:00:57 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

t_sprite	create_sprite(char *path)
{
	t_sprite	sprite;

	if (path)
	{
		sprite.img = mlx_xpm_file_to_image(get_game()->mlx, path, &sprite.width,
				&sprite.height);
		if (!sprite.img)
			exit_error(ERR_SPRITE_LOAD);
	}
	else
		sprite.img = mlx_new_image(get_game()->mlx, get_game()->window_width,
				get_game()->window_height);
	sprite.x = 0;
	sprite.y = 0;
	sprite.addr = mlx_get_data_addr(sprite.img, &sprite.bpp, &sprite.line_len,
			&sprite.endian);
	return (sprite);
}

void	draw_sprite(t_sprite *src, t_sprite *dst, int x, int y)
{
	int	sx;
	int	sy;

	sy = 0;
	while (sy < src->height)
	{
		sx = 0;
		while (sx < src->width)
		{
			if (!(sx + x > get_game()->window_width || sy
					+ y > get_game()->window_height || sx + x < 0 || sy + y < 0)
				&& (*get_sprite_pixel(src, sx, sy) != 0xFF000000))
				*get_sprite_pixel(dst, x + sx, y + sy) = *get_sprite_pixel(src,
					sx, sy);
			sx++;
		}
		sy++;
	}
}

unsigned int	*get_sprite_pixel(t_sprite *data, int x, int y)
{
	return ((unsigned int *)(data->addr + (y * data->line_len + x * (data->bpp
				/ 8))));
}

void	load_animation_sprites(t_animated_sprite *anim, char *path1,
		char *path2, int speed)
{
	anim->sprites[0] = create_sprite(path1);
	anim->sprites[1] = create_sprite(path2);
	init_animated_sprite(anim, ANIMATION_FRAMES, speed);
}

void	draw_animated_sprite(t_animated_sprite *anim, t_sprite *canvas)
{
	draw_sprite(&anim->sprites[anim->current_frame], canvas, anim->x, anim->y);
}