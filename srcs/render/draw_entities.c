/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:44:59 by paude-so          #+#    #+#             */
/*   Updated: 2025/01/02 14:50:05 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	draw_floor(void)
{
	int	x;
	int	y;

	y = -1;
	while (++y <= get_game()->window_height / SPRITE_SIZE)
	{
		x = -1;
		while (++x <= (get_game()->window_width / SPRITE_SIZE))
		{
			if ((x + y) % 2 == 0)
				draw_sprite(&get_game()->floor00, &get_game()->canvas, x
					* SPRITE_SIZE, y * SPRITE_SIZE);
			else
				draw_sprite(&get_game()->floor01, &get_game()->canvas, x
					* SPRITE_SIZE, y * SPRITE_SIZE);
		}
	}
}

void	draw_walls(void)
{
	t_wall_manager	*wall;
	int				i;

	wall = &get_game()->wall;
	update_sprite_animation(&wall->base);
	i = -1;
	while (++i < wall->count)
	{
		wall->base.x = wall->x_positions[i];
		wall->base.y = wall->y_positions[i];
		draw_sprite(&wall->base.sprites[wall->base.current_frame],
			&get_game()->canvas, wall->base.x, wall->base.y);
	}
}

void	draw_collectibles(void)
{
	t_collectible		*collectible;
	t_animated_sprite	*col_base;
	int					i;

	collectible = &get_game()->collectible;
	col_base = &collectible->base;
	i = -1;
	while (++i < collectible->count)
	{
		if (!collectible->collected[i])
		{
			collectible->base.x = collectible->x_positions[i];
			collectible->base.y = collectible->y_positions[i];
			draw_sprite(&col_base->sprites[col_base->current_frame],
				&get_game()->canvas, col_base->x, col_base->y);
		}
	}
}

void	draw_exit_part(bool draw_top)
{
	t_exit		*exit;
	t_sprite	temp;
	int			i;
	int			j;
	int			draw_y;

	exit = &get_game()->exit;
	temp = exit->sprite;
	draw_y = exit->y - (exit->sprite.height - SPRITE_SIZE);
	i = -1;
	if (!draw_top)
		i = temp.height / 2;
	while ((draw_top && ++i < temp.height / 2) || (!draw_top
			&& i < temp.height))
	{
		j = -1;
		while (++j < temp.width)
		{
			if (*get_sprite_pixel(&temp, j, i) != 0xFF000000)
				*get_sprite_pixel(&get_game()->canvas, exit->x + j, draw_y
					+ i) = *get_sprite_pixel(&temp, j, i);
		}
		if (!draw_top)
			i++;
	}
}

void	draw_exit_full(void)
{
	t_exit	*exit;
	int		draw_y;

	exit = &get_game()->exit;
	draw_y = exit->y - (exit->sprite.height - SPRITE_SIZE);
	draw_sprite(&exit->sprite, &get_game()->canvas, exit->x, draw_y);
}
