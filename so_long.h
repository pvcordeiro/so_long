/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:15:11 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/16 20:58:02 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <time.h>
# include <sys/time.h>
# include <mlx.h>
# include <X11/keysym.h>
# include <X11/X.h>
# include <stdarg.h>
# include <stdbool.h>

# define FPS 60
# define FRAME_DELAY (1000000 / FPS)
# define COLLECTIBLE_ANIMATION_SPEED 15
# define PLAYER_IDLE_ANIMATION_SPEED 20
# define PLAYER_MOVE_AND_ATTACK_ANIMATION_SPEED 10
# define FRAME_COUNT 2
# define ENEMY_ANIMATION_SPEED 20
# define WALL_ANIMATION_SPEED 100
# define BUFFER_SIZE 10
# define COLLISION_Y_OFFSET 15
# define PLAYER_SPEED 1
# define SPRINT_MULTIPLIER 2
# define SPRINT_COOLDOWN 180
# define SPRINT_DURATION 120
# define ENEMY_SPEED 1
# define ATTACK_DURATION 20
# define ATTACK_RANGE 60
# define ATTACK_COOLDOWN 40
# define INVINCIBILITY_DURATION 100
# define MOVE_COUNT_THRESHOLD 10
# define ENEMY_MOVE_THRESHOLD 50
# define ENEMY_COLLISION_WIDTH 10
# define ENEMY_COLLISION_HEIGHT 20
# define COLLECTIBLE_SIZE 20
# define WALL_COLLISION_WIDTH 30
# define WALL_COLLISION_HEIGHT 25
# define ENEMY_WALL_COLLISION_WIDTH 40
# define ENEMY_WALL_COLLISION_HEIGHT 40
# define SPRITE_SIZE 40

typedef enum e_player_state
{
    IDLE_RIGHT,
    IDLE_LEFT,
    MOVE_RIGHT,
    MOVE_LEFT,
	MOVE_UP,
	MOVE_DOWN,
	ATTACK_RIGHT,
	ATTACK_LEFT
}	t_player_state;

typedef enum e_sprite_type
{
	SPRITE_PLAYER,
	SPRITE_ENEMY,
	SPRITE_COLLECTIBLE,
	SPRITE_WALL,
	SPRITE_EXIT,
	SPRITE_HEALTH
}	t_sprite_type;

typedef struct s_sprite_config
{
	char			**paths;
	int				frame_count;
	int				anim_speed;
	int				x;
	int				y;
	t_sprite_type	type;
}	t_sprite_config;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		x;
	int		y;
	int		width;
	int		height;
}	t_img;

typedef struct s_animation
{
    t_img   sprites[4];
    int     frame_count;
    int     current_frame;
    int     anim_counter;
    int     anim_speed;
    int     x;
    int     y;
}	t_animation;

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
}	t_player;

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
}	t_enemy;

typedef struct s_enemy_list
{
	t_enemy	*enemies;
	int		count;
}	t_enemy_list;

typedef struct s_collectible
{
	t_animation  base;
	int		*x_positions;
	int		*y_positions;
	int		*collected;
	int		count;
}	t_collectible;

typedef struct s_wall
{
	t_animation 	base;
	int				*x_positions;
	int				*y_positions;
	int				count;
}	t_wall;

typedef struct s_exit
{
    t_img	sprite;
    int		x;
    int		y;
}	t_exit;

typedef struct s_map
{
    char	**map;
    int		width;
    int		height;
    int		collectibles;
    int		collectibles_reachable;
    int		exit_reachable;
}	t_map;

typedef struct s_health
{
	t_img	health1;
	t_img	health2;
	t_img	health3;
}	t_health;

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
	t_img			floor;
	t_img			floor2;
	int				collectible_count;
	int				total_collectibles;
	int				game_over;
	int				move_up;
	int				move_down;
	int				move_left;
	int				move_right;
	int				move_count;
	int				window_width;
	int				window_height;
}	t_game;

int		ft_printf(const char *input, ...);
char	*get_next_line(int fd);
t_game	*get_game(void);
t_map	*parse_map(char *filename);
char	*ft_itoa(int n);
size_t	ft_strlen(const char *s);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strdup(const char *s);
void	init_sprite(t_animation *anim, t_sprite_config config);

#endif