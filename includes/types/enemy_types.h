/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_types.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 03:09:54 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:22:34 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENEMY_TYPES_H
# define ENEMY_TYPES_H
# include "sprite_types.h"

# define ENEMY_SPEED 2
# define ENEMY_ANIMATION_SPEED 20
# define ENEMY_DIRECTION_CHANGE_DELAY 100
# define ENEMY_COLLISION_WIDTH 60
# define ENEMY_COLLISION_HEIGHT 40
# define ENEMY_INVINCIBILITY_DURATION 90
# define ENEMY_HITBOX_Y_OFFSET 25
# define ENEMY_HITBOX_X_OFFSET 10

typedef struct s_enemy
{
	t_animated_sprite	move_right;
	t_animated_sprite	move_left;
	int					y_direction;
	int					direction;
	int					move_counter;
	int					lives;
	int					invincibility_frames;
	bool				is_visible;
	bool				is_dead;
	int					x;
	int					y;
	int					state;
}						t_enemy;

typedef struct s_enemy_manager
{
	t_enemy				*enemies;
	int					count;
}						t_enemy_manager;

#endif