/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_types.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 03:10:07 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:27:30 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_TYPES_H
# define UI_TYPES_H

# define COLLECTIBLE_FRAME_DELAY 15
# define COLLECTIBLE_SIZE 40

typedef struct s_collectible
{
	t_animated_sprite	base;
	int					*x_positions;
	int					*y_positions;
	int					*collected;
	int					count;
}						t_collectible;

typedef struct s_ui_elements
{
	t_sprite			health1;
	t_sprite			health2;
	t_sprite			health3;
	t_sprite			sprint;
	t_sprite			banner;
	t_sprite			message;
}						t_ui_elements;

typedef struct s_mushroom
{
	t_sprite			sprite;
	int					x;
	int					y;
	bool				active;
	bool				collected;
}						t_mushroom;

#endif