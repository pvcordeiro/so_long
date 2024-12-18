/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:15:11 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/18 14:47:35 by paude-so         ###   ########.fr       */
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

# define SPRITE_SIZE 80
# define FRAME_DELAY 16666
# define FRAME_COUNT 2
# define COLLECTIBLE_ANIMATION_SPEED 15
# define COLLECTIBLE_SIZE 40
# define PLAYER_SPEED 2
# define PLAYER_IDLE_ANIMATION_SPEED 20
# define PLAYER_MOVE_AND_ATTACK_ANIMATION_SPEED 10
# define PLAYER_HITBOX_X_OFFSET 10
# define PLAYER_HITBOX_Y_OFFSET 25
# define PLAYER_COLLISION_WIDTH 50
# define PLAYER_COLLISION_HEIGHT 40
# define SPRINT_MULTIPLIER 2
# define SPRINT_COOLDOWN 180
# define SPRINT_DURATION 60
# define ATTACK_DURATION 20
# define ATTACK_RANGE 80
# define ATTACK_COOLDOWN 40
# define INVINCIBILITY_DURATION 100
# define BUFFER_SIZE 10
# define WALL_ANIMATION_SPEED 100
# define ENEMY_SPEED 2
# define ENEMY_ANIMATION_SPEED 20
# define ENEMY_MOVE_THRESHOLD 100
# define ENEMY_COLLISION_WIDTH 60
# define ENEMY_COLLISION_HEIGHT 40
# define HITBOX_X_OFFSET 20
# define HITBOX_Y_OFFSET 20
# define ENEMY_HITBOX_Y_OFFSET 25
# define ENEMY_HITBOX_X_OFFSET 10
# define ATTACK_HITBOX_Y_OFFSET 10

typedef enum e_player_state
{
	IDLE_RIGHT,
	IDLE_LEFT,
	MOVE_RIGHT,
	MOVE_LEFT,
	ATTACK_RIGHT,
	ATTACK_LEFT
}					t_player_state;

typedef struct s_img
{
	void			*img;
	char			*addr;
	int				bpp;
	int				line_len;
	int				endian;
	int				x;
	int				y;
	int				width;
	int				height;
}					t_img;

typedef struct s_animation
{
	t_img			sprites[5];
	int				frame_count;
	int				current_frame;
	int				anim_counter;
	int				anim_speed;
	int				x;
	int				y;
}					t_animation;

typedef struct s_player
{
	t_animation		idle_right;
	t_animation		idle_left;
	t_animation		move_right;
	t_animation		move_left;
	t_animation		attack_right;
	t_animation		attack_left;
	t_player_state	state;
	int				last_direction;
	int				current_frame;
	int				anim_counter;
	int				anim_speed;
	int				lives;
	int				invincibility_frames;
	int				attack_cooldown;
	int				max_attack_cooldown;
	int				attack_frame;
	int				attack_timer;
	bool			is_visible;
	bool			is_attacking;
	int				x;
	int				y;
	bool			is_sprinting;
	int				sprint_cooldown;
	int				sprint_duration;
	bool			can_sprint;
}					t_player;

typedef struct s_enemy
{
	t_animation		move_right;
	t_animation		move_left;
	int				y_direction;
	int				direction;
	int				move_counter;
	int				lives;
	int				invincibility_frames;
	bool			is_visible;
	bool			is_dead;
	int				x;
	int				y;
	int				state;
}					t_enemy;

typedef struct s_enemy_list
{
	t_enemy			*enemies;
	int				count;
}					t_enemy_list;

typedef struct s_collectible
{
	t_animation		base;
	int				*x_positions;
	int				*y_positions;
	int				*collected;
	int				count;
}					t_collectible;

typedef struct s_wall
{
	t_animation		base;
	int				*x_positions;
	int				*y_positions;
	int				count;
}					t_wall;

typedef struct s_exit
{
	t_img			sprite;
	int				x;
	int				y;
}					t_exit;

typedef struct s_map
{
	char			**map;
	int				width;
	int				height;
}					t_map;

typedef struct s_health
{
	t_img			health1;
	t_img			health2;
	t_img			health3;
	t_img			sprint;
	t_img			banner;
	t_img			message;
}					t_health;

typedef struct s_mushroom
{
	t_img			sprite;
	int				x;
	int				y;
	bool			active;
	bool			collected;
}					t_mushroom;

typedef struct s_game
{
	void			*mlx;
	void			*win;
	t_img			canvas;
	t_player		player;
	t_collectible	collectible;
	t_wall			wall;
	t_enemy_list	enemy_list;
	t_exit			exit;
	t_map			map;
	t_health		health;
	t_mushroom		mushroom;
	t_img			floor;
	t_img			floor2;
	t_img			victory;
	t_img			defeat;
	int				collectible_count;
	int				move_up;
	int				move_down;
	int				move_left;
	int				move_right;
	int				move_count;
	int				window_width;
	int				window_height;
	bool			vic;
	bool			game_over;
}					t_game;

int					ft_printf(const char *input, ...);
char				*get_next_line(int fd);
t_game				*get_game(void);
t_map				*parse_map(char *filename);
char				*ft_itoa(int n);
size_t				ft_strlen(const char *s);
char				*ft_strchr(const char *s, int c);
char				*ft_strrchr(const char *s, int c);
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_strdup(const char *s);
void				init_entity_position(t_map *map, char c, int *x, int *y);
void				handle_entity_collision(int *x, int *y, int prev_x,
						int prev_y);
void				load_player_animations(t_player *player);
int					count_map_char(char **map, int height, int width, char c);
void				load_animation_sprites(t_animation *anim, char *path1,
						char *path2, int speed);
void				load_player_animations(t_player *player);
void				update_enemy_state(t_enemy *enemy);
void				update_enemy_movement(t_enemy *enemy);
void				init_enemy_state(t_enemy *enemy, int x, int y);
void				init_enemy_animations(t_enemy *enemy);
int					check_wall_collisions(int x, int y, int width, int height);
int					check_enemy_collisions(int x, int y, int current_enemy);
void				update_animation(t_animation *anim);
void				update_game_state(void);
void				draw_background_layer(void);
void				draw_entity_layer(void);
void				draw_player_and_exit(void);
void				draw_ui_layer(void);
void				handle_game_state(void);
void				update_mushroom(void);
void				update_collectible(void);
void				update_enemy(void);
void				check_attack_collision(void);
void				update_player_position(void);
void				update_player(void);
void				victory_check(void);
void				draw_floor(void);
void				draw_collectibles(void);
void				draw_walls(void);
void				draw_enemy(void);
void				draw_player(void);
void				draw_exit_full(void);
void				draw_exit_bottom(void);
void				draw_exit_top(void);
void				draw_health(void);
void				init_health(void);
void				draw_animation(t_animation *anim, t_img *canvas);
void				draw_mushroom(void);
void				helper_message(void);
void				draw_ui_banners(void);
void				draw_end_game_screen(void);
void				draw_sprint_icon(void);
void				draw_collectible_counter(void);
void				update_entities(void);
void				draw_frame(void);
void				init_game(char *map_path);
void				cleanup_sprites(void);
void				init_window(void);
void				init_sprites(void);
void				setup_hooks(void);
void				draw_text_layer(void);

#endif