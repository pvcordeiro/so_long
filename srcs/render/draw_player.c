/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:42:36 by paude-so          #+#    #+#             */
/*   Updated: 2025/01/02 14:29:21 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	load_player_animations(t_player *player)
{
	load_animation_sprites(&player->idle_right,
		"assets/player/idle_right/player_idle_right00.xpm",
		"assets/player/idle_right/player_idle_right01.xpm",
		PLAYER_IDLE_FRAME_DELAY);
	load_animation_sprites(&player->idle_left,
		"assets/player/idle_left/player_idle_left00.xpm",
		"assets/player/idle_left/player_idle_left01.xpm",
		PLAYER_IDLE_FRAME_DELAY);
	load_animation_sprites(&player->move_right,
		"assets/player/move_right/player_move_right00.xpm",
		"assets/player/move_right/player_move_right01.xpm",
		PLAYER_IDLE_FRAME_DELAY);
	load_animation_sprites(&player->move_left,
		"assets/player/move_left/player_move_left00.xpm",
		"assets/player/move_left/player_move_left01.xpm",
		PLAYER_IDLE_FRAME_DELAY);
	load_animation_sprites(&player->attack_right,
		"assets/player/attack_right/player_attack_right00.xpm",
		"assets/player/attack_right/player_attack_right01.xpm",
		PLAYER_IDLE_FRAME_DELAY);
	load_animation_sprites(&player->attack_left,
		"assets/player/attack_left/player_attack_left00.xpm",
		"assets/player/attack_left/player_attack_left01.xpm",
		PLAYER_IDLE_FRAME_DELAY);
}

static bool	is_player_visible(t_player *player)
{
	if (player->invincibility_frames > 0)
	{
		player->is_visible = !player->is_visible;
		return (player->is_visible);
	}
	return (true);
}

static t_animated_sprite	*get_player_animation(t_player *player)
{
	if (player->state == IDLE_RIGHT)
		return (&player->idle_right);
	if (player->state == IDLE_LEFT)
		return (&player->idle_left);
	if (player->state == MOVE_RIGHT)
		return (&player->move_right);
	if (player->state == MOVE_LEFT)
		return (&player->move_left);
	if (player->state == ATTACK_RIGHT)
		return (&player->attack_right);
	return (&player->attack_left);
}

void	draw_player(void)
{
	t_player			*player;
	t_animated_sprite	*current_anim;

	player = &get_game()->player;
	if (!is_player_visible(player))
		return ;
	current_anim = get_player_animation(player);
	current_anim->x = player->x;
	current_anim->y = player->y;
	draw_sprite(&current_anim->sprites[current_anim->current_frame],
		&get_game()->canvas, current_anim->x, current_anim->y);
}
