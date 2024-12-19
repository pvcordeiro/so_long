/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:15:11 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:19:32 by paude-so         ###   ########.fr       */
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

# define SPRITE_SIZE 80
# define FRAME_TIME_MS 16666
# define ANIMATION_FRAMES 2
# define COLLECTIBLE_FRAME_DELAY 15
# define COLLECTIBLE_SIZE 40
# define ATTACK_COLLISION_Y_OFFSET 10
# define SPRINT_MULTIPLIER 2
# define SPRINT_COOLDOWN 180
# define SPRINT_DURATION 60
# define ATTACK_DURATION 20
# define ATTACK_RANGE 80
# define ATTACK_COOLDOWN 40
# define BUFFER_SIZE 10
# define WALL_ANIMATION_SPEED 100
# define ENEMY_SPEED 2
# define ENEMY_ANIMATION_SPEED 20
# define ENEMY_DIRECTION_CHANGE_DELAY 100
# define ENEMY_COLLISION_WIDTH 60
# define ENEMY_COLLISION_HEIGHT 40
# define ENEMY_INVINCIBILITY_DURATION 90
# define ENEMY_HITBOX_Y_OFFSET 25
# define ENEMY_HITBOX_X_OFFSET 10
# define HITBOX_X_OFFSET 20
# define HITBOX_Y_OFFSET 20

typedef struct s_map_entity_counts
{
	int					player;
	int					exit;
	int					collect;
	int					empty;
}						t_map_entity_counts;

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

typedef struct s_collectible
{
	t_animated_sprite	base;
	int					*x_positions;
	int					*y_positions;
	int					*collected;
	int					count;
}						t_collectible;

typedef struct s_wall_manager
{
	t_animated_sprite	base;
	int					*x_positions;
	int					*y_positions;
	int					count;
}						t_wall_manager;

typedef struct s_exit
{
	t_sprite			sprite;
	int					x;
	int					y;
}						t_exit;

typedef struct s_map
{
	char				**map;
	int					width;
	int					height;
}						t_map;

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

typedef struct s_game_state
{
	void				*mlx;
	void				*win;
	t_sprite			canvas;
	t_player			player;
	t_collectible		collectible;
	t_wall_manager		wall;
	t_enemy_manager		enemy_list;
	t_exit				exit;
	t_map				map;
	t_ui_elements		health;
	t_mushroom			mushroom;
	t_sprite			floor;
	t_sprite			floor2;
	t_sprite			victory;
	t_sprite			defeat;
	int					collectible_count;
	int					move_up;
	int					move_down;
	int					move_left;
	int					move_right;
	int					move_count;
	int					window_width;
	int					window_height;
	bool				vic;
	bool				game_over;
}						t_game_state;

int						game_loop(void);
int						exit_game_state(void);
int						exit_error(t_error error);
void					init_game_state(char *map_path);
t_game_state			*get_game(void);






t_error					validate_map(t_map *map);
void					init_player(void);
void					init_enemy(void);
void					init_enemy_state(t_enemy *enemy, int x, int y);
void					init_enemy_animations(t_enemy *enemy);
void					init_exit(void);
void					update_player(void);
void					update_player_position(void);
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
void					handle_movement_keys(t_game_state *game, int key,
							char *action);
void					handle_movement(t_player *player, int movement_speed,
							int *prev_x, int *prev_y);
void					handle_sprint_key(t_game_state *game, char *action);
void					handle_sprint_cooldown(t_player *player);
void					handle_sprint(t_player *player, int *movement_speed);
void					handle_enemy_collision(t_enemy *enemy, int prev_x,
							int prev_y);
void					handle_attack_key(t_player *player);
void					init_wall_manager(void);
void					init_collectible(void);
void					init_mushroom(void);
void					draw_mushroom(void);

void					victory_check(void);
bool					check_path(t_map *map);
char					**create_temp_map(t_map *map);
void					draw_enemy(void);
void					draw_player(void);
void					load_player_animations(t_player *player);
int						ft_printf(const char *input, ...);
char					*get_next_line(int fd);
t_map					*parse_map(char *filename);
char					*ft_itoa(int n);
size_t					ft_strlen(const char *s);
char					*ft_strrchr(const char *s, int c);
int						ft_strcmp(const char *s1, const char *s2);
char					*ft_strdup(const char *s);
void					init_entity_position(t_map *map, char c, int *x,
							int *y);
void					handle_entity_collision(int *x, int *y, int prev_x,
							int prev_y);
int						count_map_char(char **map, int height, int width,
							char c);
int						check_wall_collisions(int x, int y, int width,
							int height);
int						check_enemy_collisions(int x, int y, int current_enemy);
void					handle_game_state(void);
void					check_attack_collision(void);
void					draw_floor(void);
void					draw_collectibles(void);
void					draw_walls(void);
void					draw_exit_full(void);
void					draw_exit_bottom(void);
void					draw_exit_top(void);
void					init_ui_elements(void);
void					draw_end_game_screen(void);

#endif