/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_sprite.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 01:04:11 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 19:55:46 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

static void	cleanup_dynamic_memory(t_game_state *game)
{
	int	i;

	free(game->wall.x_positions);
	free(game->wall.y_positions);
	free(game->collectible.x_positions);
	free(game->collectible.y_positions);
	free(game->collectible.collected);

	i = -1;
	while(++i < game->map.height)
		free(game->map.map[i]);
	free(game->map.map);
}

static void	cleanup_enemy_sprites(t_game_state *game)
{
	int	i;
	int	j;

	i = -1;
	while (++i < game->enemy_list.count)
	{
		j = -1;
		while (++j < 2)
		{
			mlx_destroy_image(game->mlx,
				game->enemy_list.enemies[i].move_right.sprites[j].img);
			mlx_destroy_image(game->mlx,
				game->enemy_list.enemies[i].move_left.sprites[j].img);
		}
	}
	free(game->enemy_list.enemies);
}

static void	cleanup_player_and_col_sprites(t_game_state *game)
{
	int	i;

	i = -1;
	while (++i < 4)
		mlx_destroy_image(game->mlx, game->collectible.base.sprites[i].img);
	i = -1;
	while (++i < 2)
	{
		mlx_destroy_image(game->mlx, game->wall.base.sprites[i].img);
		mlx_destroy_image(game->mlx, game->player.idle_right.sprites[i].img);
		mlx_destroy_image(game->mlx, game->player.idle_left.sprites[i].img);
		mlx_destroy_image(game->mlx, game->player.move_right.sprites[i].img);
		mlx_destroy_image(game->mlx, game->player.move_left.sprites[i].img);
		mlx_destroy_image(game->mlx, game->player.attack_right.sprites[i].img);
		mlx_destroy_image(game->mlx, game->player.attack_left.sprites[i].img);
	}
}

static void	cleanup_misc_sprites(t_game_state *game)
{
	mlx_destroy_image(game->mlx, game->canvas.img);
	mlx_destroy_image(game->mlx, game->floor.img);
	mlx_destroy_image(game->mlx, game->floor2.img);
	mlx_destroy_image(game->mlx, game->mushroom.sprite.img);
	mlx_destroy_image(game->mlx, game->exit.sprite.img);
	mlx_destroy_image(game->mlx, game->ui.health1.img);
	mlx_destroy_image(game->mlx, game->ui.health2.img);
	mlx_destroy_image(game->mlx, game->ui.health3.img);
	mlx_destroy_image(game->mlx, game->ui.sprint.img);
	mlx_destroy_image(game->mlx, game->ui.banner.img);
	mlx_destroy_image(game->mlx, game->ui.message.img);
}

void	cleanup_sprites(void)
{
	t_game_state	*game;

	game = get_game();
	cleanup_dynamic_memory(game);
	cleanup_enemy_sprites(game);
	cleanup_player_and_col_sprites(game);
	cleanup_misc_sprites(game);
}
