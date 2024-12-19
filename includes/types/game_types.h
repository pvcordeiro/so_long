/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_types.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 03:41:08 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:41:13 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_TYPES_H
# define GAME_TYPES_H

# include "sprite_types.h"
# include "player_types.h"
# include "enemy_types.h"
# include "map_types.h"
# include "ui_types.h"

typedef struct s_game_state
{
    void            *mlx;
    void            *win;
    t_sprite        canvas;
    t_player        player;
    t_collectible   collectible;
    t_wall_manager  wall;
    t_enemy_manager enemy_list;
    t_exit          exit;
    t_map           map;
    t_ui_elements   health;
    t_mushroom      mushroom;
    t_sprite        floor;
    t_sprite        floor2;
    t_sprite        victory;
    t_sprite        defeat;
    int             collectible_count;
    int             move_up;
    int             move_down;
    int             move_left;
    int             move_right;
    int             move_count;
    int             window_width;
    int             window_height;
    bool            vic;
    bool            game_over;
}                   t_game_state;

#endif