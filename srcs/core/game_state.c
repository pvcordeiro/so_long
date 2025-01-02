/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_state.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:46:57 by paude-so          #+#    #+#             */
/*   Updated: 2025/01/02 12:26:39 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

t_game_state	*get_game(void)
{
	static t_game_state	game;

	return (&game);
}

int	exit_game(void)
{
	if (get_game()->mlx)
	{
		cleanup_sprites();
		mlx_destroy_window(get_game()->mlx, get_game()->win);
		mlx_destroy_display(get_game()->mlx);
		free(get_game()->mlx);
	}
	ft_printf("Game closed\n");
	exit(EXIT_SUCCESS);
}

static char	*get_error_message(t_error error)
{
	char	**messages;

	messages = (char *[]){
		"",
		"Error\nMap must be surrounded by walls",
		"Error\nNo player (P) found in map",
		"Error\nMultiple players found in map",
		"Error\nNo exit (E) found in map",
		"Error\nMultiple exits found in map",
		"Error\nNo collectibles (C) found in map",
		"Error\nNo valid path to exit/collectibles",
		"Error\nInvalid character in map",
		"Error\nMap is not rectangular"
	};
	return (messages[error]);
}

int	exit_error(t_error error)
{
	if (get_game()->mlx)
	{
		cleanup_sprites();
		mlx_destroy_window(get_game()->mlx, get_game()->win);
		mlx_destroy_display(get_game()->mlx);
		free(get_game()->mlx);
	}
	ft_printf("%s\n", get_error_message(error));
	exit(EXIT_FAILURE);
}
