/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:15:11 by paude-so          #+#    #+#             */
/*   Updated: 2025/01/02 11:15:14 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <X11/X.h>
# include <X11/keysym.h>
# include <fcntl.h>
# include "../minilibx-linux/mlx.h"
# include <stdarg.h>
# include <stdbool.h>
# include <stdlib.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

# include "types/common_types.h"
# include "types/game_types.h"
# include "types/sprite_types.h"
# include "types/player_types.h"
# include "types/enemy_types.h"
# include "types/map_types.h"
# include "types/ui_types.h"

# include "core/sprite.h"
# include "core/player.h"
# include "core/enemy.h"
# include "core/map.h"
# include "core/ui.h"
# include "core/utils.h"
# include "core/collision.h"

# define BUFFER_SIZE 10

int						game_loop(void);
int						exit_game(void);
int						exit_error(t_error error);
void					init_game_state(char *map_path);
t_game_state			*get_game(void);
void					handle_game_state(void);
void					update_entities(void);
void					victory_check(void);

#endif