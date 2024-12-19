/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 03:10:15 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:51:28 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPRITE_H
# define SPRITE_H

# include "../types/sprite_types.h"

t_sprite		create_sprite(char *path);
void			draw_sprite(t_sprite *src, t_sprite *dst, int x, int y);
void			cleanup_sprites(void);
t_sprite		create_sprite(char *path);
void			draw_sprite(t_sprite *src, t_sprite *dst, int x, int y);
void			draw_animated_sprite(t_animated_sprite *anim, t_sprite *canvas);
void			init_animated_sprite(t_animated_sprite *anim, int frame_count,
					int speed);
void			update_sprite_animation(t_animated_sprite *anim);
void			load_animation_sprites(t_animated_sprite *anim, char *path1,
					char *path2, int speed);
unsigned int	*get_sprite_pixel(t_sprite *data, int x, int y);
void			init_collectible(void);
void			init_mushroom(void);
void			init_exit(void);

#endif