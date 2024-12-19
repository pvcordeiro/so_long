/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_types.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 03:10:04 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:15:18 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPRITE_TYPES_H
# define SPRITE_TYPES_H

# define SPRITE_SIZE 80
# define FRAME_TIME_MS 16666
# define ANIMATION_FRAMES 2

typedef struct s_sprite
{
	void		*img;
	char		*addr;
	int			bpp;
	int			line_len;
	int			endian;
	int			x;
	int			y;
	int			width;
	int			height;
}				t_sprite;

typedef struct s_animated_sprite
{
	t_sprite	sprites[5];
	int			frame_count;
	int			current_frame;
	int			anim_counter;
	int			anim_speed;
	int			x;
	int			y;
}				t_animated_sprite;

#endif