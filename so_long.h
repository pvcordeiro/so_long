/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:15:11 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/13 21:47:56 by paude-so         ###   ########.fr       */
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

typedef struct s_game
{
	void    *mlx;
	void    *win;
	t_img	canvas;
	t_img	player;
	t_img	player2;
	t_img	floor;
	t_img	floor2;
	t_img	wall;
	t_img	exit;
	t_img	collectible;
	int		move_up;
	int		move_down;
	int		move_left;
	int		move_right;
}			t_game;

# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 400
# define FPS 60
# define FRAME_DELAY (1000000 / FPS)
#define _POSIX_C_SOURCE 200809L

int	ft_printf(const char *input, ...);
t_game	*get_game(void);

#endif