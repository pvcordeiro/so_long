/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:14:46 by paude-so          #+#    #+#             */
/*   Updated: 2025/01/02 11:58:57 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static bool	check_file_extension(char *filename)
{
	char	*dot;

	dot = ft_strrchr(filename, '.');
	if (!dot || ft_strcmp(dot, ".ber") != 0)
		return (false);
	return (true);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (ft_printf("Error\nUsage: ./so_long [map.ber]\n"));
	if (!check_file_extension(argv[1]))
		return (ft_printf("Error\nFile must have .ber extension\n"));
	srand(time(NULL));
	init_game_state(argv[1]);
	mlx_loop(get_game()->mlx);
	return (0);
}
