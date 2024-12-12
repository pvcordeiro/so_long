/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:15:11 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/12 14:17:00 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <mlx.h>
# include <X11/keysym.h>
# include <X11/X.h>
# include <stdlib.h>
# include <stdarg.h>
# include <unistd.h>

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
	t_img	floor;
}			t_game;

# define WINDOW_WIDTH 500
# define WINDOW_HEIGHT 500

int	ft_printf(const char *input, ...);
t_game	*get_game(void);

#endif