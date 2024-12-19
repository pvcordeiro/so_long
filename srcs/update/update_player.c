/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:46:09 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 01:30:05 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	update_player_position(void)
{
	int			prev_x;
	int			prev_y;
	t_player	*player;
	static int	distance_moved = 0;
	int			movement_speed;

	player = &get_game()->player;
	prev_x = player->x;
	prev_y = player->y;
	movement_speed = PLAYER_MOVE_SPEED;
	if (player->is_sprinting)
		handle_sprint(player, &movement_speed);
	handle_sprint_cooldown(player);
	if (!player->is_attacking)
		handle_movement(player, movement_speed, &prev_x, &prev_y);
	if (prev_x != player->x || prev_y != player->y)
	{
		distance_moved += ft_abs(player->x - prev_x) + ft_abs(player->y
				- prev_y);
		if (distance_moved >= SPRITE_SIZE)
		{
			get_game()->move_count++;
			distance_moved = 0;
		}
	}
}

static void	handle_player_attack(t_player *player,
		t_animated_sprite *current_anim)
{
	player->attack_timer++;
	if (player->state == ATTACK_RIGHT)
		current_anim = &player->attack_right;
	else
		current_anim = &player->attack_left;
	if (player->attack_timer < 10)
		current_anim->current_frame = 0;
	else if (player->attack_timer < 20)
		current_anim->current_frame = 1;
	else
	{
		player->is_attacking = false;
		player->attack_cooldown = player->max_attack_cooldown;
		player->attack_timer = 0;
		if (player->state == ATTACK_RIGHT)
			player->state = IDLE_RIGHT;
		else
			player->state = IDLE_LEFT;
	}
}

static void	update_player_state(t_player *player)
{
	if (get_game()->move_right)
	{
		player->state = MOVE_RIGHT;
		player->last_direction = MOVE_RIGHT;
	}
	else if (get_game()->move_left)
	{
		player->state = MOVE_LEFT;
		player->last_direction = MOVE_LEFT;
	}
	else if (get_game()->move_up || get_game()->move_down)
	{
		if (player->last_direction == MOVE_RIGHT)
			player->state = MOVE_RIGHT;
		else
			player->state = MOVE_LEFT;
	}
	else
	{
		if (player->last_direction == MOVE_RIGHT)
			player->state = IDLE_RIGHT;
		else
			player->state = IDLE_LEFT;
	}
}

void	update_player(void)
{
	t_player			*player;
	t_animated_sprite	*current_anim;

	player = &get_game()->player;
	if (player->attack_cooldown > 0)
		player->attack_cooldown--;
	if (player->is_attacking)
	{
		handle_player_attack(player, current_anim);
		return ;
	}
	update_player_state(player);
	if (player->state == IDLE_RIGHT)
		current_anim = &player->idle_right;
	else if (player->state == IDLE_LEFT)
		current_anim = &player->idle_left;
	else if (player->state == MOVE_RIGHT)
		current_anim = &player->move_right;
	else
		current_anim = &player->move_left;
	update_sprite_animation(current_anim);
}
