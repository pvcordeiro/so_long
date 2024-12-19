/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_types.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 03:13:56 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:29:19 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_TYPES_H
# define COMMON_TYPES_H

# define HITBOX_X_OFFSET 20
# define HITBOX_Y_OFFSET 20

typedef struct s_position
{
    int x;
    int y;
} t_position;

typedef enum e_error
{
	ERR_NONE = 0,
	ERR_WALLS,
	ERR_NO_PLAYER,
	ERR_MULTIPLE_PLAYERS,
	ERR_NO_EXIT,
	ERR_MULTIPLE_EXITS,
	ERR_NO_COLLECTIBLES,
	ERR_NO_EMPTY_SPACE,
	ERR_INVALID_PATH,
	ERR_INVALID_CHAR,
	ERR_MAP_NOT_RECTANGULAR,
	ERR_MEMORY,
	ERR_SPRITE_LOAD
}						t_error;

#endif