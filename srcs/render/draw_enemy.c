/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_enemy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:40:17 by paude-so          #+#    #+#             */
/*   Updated: 2025/01/02 14:27:58 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

static bool	is_enemy_visible(t_enemy *enemy)
{
	if (enemy->invincibility_frames > 0)
	{
		enemy->is_visible = !enemy->is_visible;
		return (enemy->is_visible);
	}
	return (true);
}

static t_animated_sprite	*get_enemy_animation(t_enemy *enemy)
{
	if (enemy->state == MOVE_RIGHT)
		return (&enemy->move_right);
	return (&enemy->move_left);
}

static void	draw_single_enemy(t_enemy *enemy)
{
	t_animated_sprite	*current_anim;

	if (!enemy->is_dead && is_enemy_visible(enemy))
	{
		current_anim = get_enemy_animation(enemy);
		current_anim->x = enemy->x;
		current_anim->y = enemy->y;
		draw_sprite(&current_anim->sprites[current_anim->current_frame],
			&get_game()->canvas, current_anim->x, current_anim->y);
	}
}

void	draw_enemy(void)
{
	t_enemy_manager	*enemy_list;
	int				i;

	enemy_list = &get_game()->enemy_list;
	i = -1;
	while (++i < enemy_list->count)
		draw_single_enemy(&enemy_list->enemies[i]);
}
