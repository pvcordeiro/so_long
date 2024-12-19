/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ui.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:45:27 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 01:48:02 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	draw_health(void)
{
	t_ui_elements	*health;
	t_sprite		*current_sprite;

	health = &get_game()->health;
	if (get_game()->player.lives == 3)
		current_sprite = &health->health3;
	else if (get_game()->player.lives == 2)
		current_sprite = &health->health2;
	else if (get_game()->player.lives == 1)
		current_sprite = &health->health1;
	else
		return ;
	draw_sprite(current_sprite, &get_game()->canvas, 10, -10);
}

void	draw_sprint_icon(void)
{
	t_player	*player;
	static int	flash_counter = 0;
	int			sprint_x;
	int			sprint_y;

	player = &get_game()->player;
	sprint_x = get_game()->window_width - 80;
	sprint_y = get_game()->window_height - 80;
	if (player->is_sprinting)
		draw_sprite(&get_game()->health.sprint, &get_game()->canvas, sprint_x,
			sprint_y);
	else if (!player->can_sprint)
	{
		flash_counter++;
		if ((flash_counter / 15) % 2)
			draw_sprite(&get_game()->health.sprint, &get_game()->canvas,
				sprint_x, sprint_y);
	}
	else
		draw_sprite(&get_game()->health.sprint, &get_game()->canvas, sprint_x,
			sprint_y);
}

void	draw_ui_banners(void)
{
	int	banner_top_right;
	int	banner_bottom_left;
	int	banner_bottom_right;

	banner_top_right = get_game()->window_width - 80;
	draw_sprite(&get_game()->health.banner, &get_game()->canvas,
		banner_top_right, 0);
	banner_bottom_left = 0;
	draw_sprite(&get_game()->health.banner, &get_game()->canvas,
		banner_bottom_left, get_game()->window_height - 80);
	banner_bottom_right = get_game()->window_width - 80;
	draw_sprite(&get_game()->health.banner, &get_game()->canvas,
		banner_bottom_right, get_game()->window_height - 80);
}

void	draw_collectible_counter(void)
{
	t_collectible	*collectible;
	int				x;
	int				y;

	x = get_game()->window_width - 80;
	y = 0;
	collectible = &get_game()->collectible;
	draw_sprite(&collectible->base.sprites[4], &get_game()->canvas, x, y);
}

void	draw_helper_message(void)
{
	int			x;
	int			y;
	t_position	pos1;
	t_position	pos2;

	pos1.x = get_game()->player.x;
	pos1.y = get_game()->player.y;
	pos2.x = get_game()->exit.x;
	pos2.y = get_game()->exit.y;
	if (check_collision(pos1, pos2, 40, 40)
		&& get_game()->collectible_count < get_game()->collectible.count)
	{
		x = get_game()->exit.x + (SPRITE_SIZE - 160) / 2;
		y = get_game()->exit.y - 40;
		draw_sprite(&get_game()->health.message, &get_game()->canvas, x, y);
	}
}
