/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_types.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 03:10:01 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:16:44 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_TYPES_H
# define PLAYER_TYPES_H
# include "sprite_types.h"

# define PLAYER_MOVE_SPEED 2
# define PLAYER_IDLE_FRAME_DELAY 20
# define PLAYER_COLLISION_X_OFFSET 10
# define PLAYER_COLLISION_Y_OFFSET 25
# define PLAYER_COLLISION_WIDTH 50
# define PLAYER_COLLISION_HEIGHT 50
# define PLAYER_INVINCIBILITY_DURATION 180

typedef enum e_player_state
{
	IDLE_RIGHT,
	IDLE_LEFT,
	MOVE_RIGHT,
	MOVE_LEFT,
	ATTACK_RIGHT,
	ATTACK_LEFT
}						t_player_state;

typedef struct s_player
{
	t_animated_sprite	idle_right;
	t_animated_sprite	idle_left;
	t_animated_sprite	move_right;
	t_animated_sprite	move_left;
	t_animated_sprite	attack_right;
	t_animated_sprite	attack_left;
	t_player_state		state;
	int					last_direction;
	int					lives;
	int					invincibility_frames;
	int					attack_cooldown;
	int					max_attack_cooldown;
	int					attack_frame;
	int					attack_timer;
	bool				is_visible;
	bool				is_attacking;
	int					x;
	int					y;
	bool				is_sprinting;
	int					sprint_cooldown;
	int					sprint_duration;
	bool				can_sprint;
}						t_player;

#endif