/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:14:46 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/07 10:51:22 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

#include <mlx.h>

int	main(void)
{
	void	*mlx;
	void	*mlx_win;

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 500, 500, "so_long");
	mlx_loop(mlx);
}


// typedef struct s_game
// {
// 	void    *mlx;
// 	void    *win;
// 	// Add more game-related variables here
// } t_game;

// int close_window(t_game *game)
// {
// 	mlx_destroy_window(game->mlx, game->win);
// 	exit(0);
// 	return (0);
// }

// int main(int argc, char **argv)
// {
// 	t_game  game;

// 	if (argc != 2)
// 	{
// 		ft_printf("Usage: ./so_long map.ber\n");
// 		return (1);
// 	}

// 	game.mlx = mlx_init();
// 	if (!game.mlx)
// 		return (1);
	
// 	game.win = mlx_new_window(game.mlx, 800, 600, "so_long");
// 	if (!game.win)
// 		return (1);

// 	mlx_hook(game.win, 17, 0, close_window, &game);
// 	mlx_loop(game.mlx);

// 	return (0);
// }