/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:46:47 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 19:58:11 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

static int	key_loop(int key, char *action)
{
	t_game_state	*game;

	game = get_game();
	if (key == XK_Escape)
		exit_game();
	if (!game->vic && !game->game_over)
	{
		if (key == XK_Shift_L)
			handle_sprint_key(game, action);
		handle_movement_keys(game, key, action);
		if (key == XK_space && *action == 'p')
			handle_attack_key(&game->player);
	}
	return (0);
}

static void	setup_hooks(void)
{
	mlx_loop_hook(get_game()->mlx, game_loop, NULL);
	mlx_hook(get_game()->win, KeyPress, KeyPressMask, key_loop, "p");
	mlx_hook(get_game()->win, KeyRelease, KeyReleaseMask, key_loop, "r");
	mlx_hook(get_game()->win, DestroyNotify, KeyPressMask, exit_game,
		NULL);
}

static void	init_sprites(void)
{
	get_game()->canvas = create_sprite(NULL);
	get_game()->floor = create_sprite("assets/terrain/floor00.xpm");
	get_game()->floor2 = create_sprite("assets/terrain/floor01.xpm");
	get_game()->victory = create_sprite("assets/misc/victory.xpm");
	get_game()->defeat = create_sprite("assets/misc/defeat.xpm");
	init_collectible();
	init_wall_manager();
	init_player();
	init_enemy();
	init_exit();
	init_ui_elements();
	init_mushroom();
}

static void	init_window(void)
{
	t_game_state	*game;

	game = get_game();
	game->window_width = game->map.width * SPRITE_SIZE;
	game->window_height = game->map.height * SPRITE_SIZE;
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, game->window_width,
			game->window_height, "so_long");
	game->game_over = false;
	game->vic = false;
	if (!game->mlx || !game->win)
		exit_error(ERR_MEMORY);
}

void	init_game_state(char *map_path)
{
	t_game_state	*game;
	t_map			*map_info;

	game = get_game();
	game->mlx = NULL;
	game->win = NULL;
	map_info = parse_map(map_path);
	if (!map_info)
		exit_error(ERR_MEMORY);
	game->map = *map_info;
	free(map_info);
	init_window();
	init_sprites();
	setup_hooks();
}
