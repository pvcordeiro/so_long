/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:15:11 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:42:04 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <X11/X.h>
# include <X11/keysym.h>
# include <fcntl.h>
# include <mlx.h>
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

# define BUFFER_SIZE 10

int						game_loop(void);
int						exit_game_state(void);
int						exit_error(t_error error);
void					init_game_state(char *map_path);
t_game_state			*get_game(void);
void					handle_game_state(void);





void					init_enemy(void);
void					init_enemy_state(t_enemy *enemy, int x, int y);
void					init_enemy_animations(t_enemy *enemy);
void					init_exit(void);
void					update_enemy(void);
void					update_entities(void);
void					draw_health(void);
void					draw_ui_banners(void);
void					draw_sprint_icon(void);
void					draw_helper_message(void);
void					draw_collectible_counter(void);
unsigned int			*get_sprite_pixel(t_sprite *data, int x, int y);
int						check_collision(t_position pos1, t_position pos2,
							int width, int height);

void					draw_frame(void);
void					handle_enemy_collision(t_enemy *enemy, int prev_x,
							int prev_y);
void					init_collectible(void);
void					init_mushroom(void);
void					draw_mushroom(void);

void					victory_check(void);
void					draw_enemy(void);
int						ft_printf(const char *input, ...);
char					*get_next_line(int fd);
char					*ft_itoa(int n);
size_t					ft_strlen(const char *s);
char					*ft_strrchr(const char *s, int c);
int						ft_strcmp(const char *s1, const char *s2);
char					*ft_strdup(const char *s);
void					handle_entity_collision(int *x, int *y, int prev_x,
							int prev_y);
int						check_enemy_collisions(int x, int y, int current_enemy);
void					check_attack_collision(void);
void					draw_collectibles(void);
void					draw_exit_full(void);
void					draw_exit_bottom(void);
void					draw_exit_top(void);
void					init_ui_elements(void);
void					draw_end_game_screen(void);

#endif