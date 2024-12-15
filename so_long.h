/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:15:11 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/15 16:35:46 by paude-so         ###   ########.fr       */
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

typedef enum e_player_state
{
    IDLE_RIGHT,
    IDLE_LEFT,
    MOVE_RIGHT,
    MOVE_LEFT,
	MOVE_UP,
	MOVE_DOWN
} t_player_state;

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
}			t_img;

typedef struct s_animation
{
    t_img   sprites[4];
    int     frame_count;
    int     current_frame;
    int     anim_counter;
    int     anim_speed;
    int     x;
    int     y;
} t_animation;

typedef struct s_player
{
    t_animation  idle_right;
	t_animation  idle_left;
	t_animation  move_right;
	t_animation  move_left;
    t_player_state state;
	int		last_direction;
    int     current_frame;
    int     anim_counter;
    int     anim_speed;
	int		lives;
	int		invincibility_frames;
	int		is_visible;
    int     x;
    int     y;
} t_player;

typedef struct s_enemy
{
    t_animation  idle_right;
    t_animation  idle_left;
    t_animation  move_right;
    t_animation  move_left;
    t_player_state state;
	int	y_direction;
    int direction;
    int move_counter;
    int x;
    int y;
} t_enemy;

typedef struct s_collectible
{
	t_animation  base;
	int		collected;
} t_collectible;

typedef struct s_wall
{
	t_animation  base;
} t_wall;

typedef struct s_exit
{
    t_img   sprite;
    int     x;
    int     y;
} t_exit;

typedef struct s_map {
    char **map;
    int width;
    int height;
    int collectibles;
    int collectibles_reachable;
    int exit_reachable;
} t_map;

typedef struct s_health
{
	t_img	health1;
	t_img	health2;
	t_img	health3;
}	t_health;

typedef struct s_game
{
	void    *mlx;
	void    *win;
	t_img	canvas;
	t_player	player;
	t_collectible	collectible;
	t_wall	wall;
	t_enemy	enemy;
	t_exit	exit;
	t_map	map;
	t_health	health;
	t_img	floor;
	t_img	floor2;
	int		collectible_count;
	int		total_collectibles;
	int		game_over;
	int		move_up;
	int		move_down;
	int		move_left;
	int		move_right;
	int		move_count;
}			t_game;

# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 800
# define FPS 60
# define FRAME_DELAY (1000000 / FPS)
# define ANIMATION_SPEED 15
# define BUFFER_SIZE 10
# define COLLISION_Y_OFFSET 5

int	ft_printf(const char *input, ...);
char	*get_next_line(int fd);
t_game	*get_game(void);
t_map *parse_map(char *filename);
char *ft_itoa(int n);
size_t ft_strlen(const char *s);
char *ft_strchr(const char *s, int c);
char *ft_strrchr(const char *s, int c);
int ft_strcmp(const char *s1, const char *s2);
char *ft_strdup(const char *s);

#endif