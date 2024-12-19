/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_collectibles.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 01:26:41 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 20:09:33 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

static int	count_collectible_positions(t_map *map)
{
	int	count;
	int	i;
	int	j;

	count = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
			if (map->map[i][j] == 'C')
				count++;
	}
	return (count);
}

static bool	allocate_collectible_arrays(t_collectible *collectible, int count)
{
	collectible->x_positions = malloc(sizeof(int) * count);
	collectible->y_positions = malloc(sizeof(int) * count);
	collectible->collected = malloc(sizeof(int) * count);
	if (!collectible->x_positions || !collectible->y_positions
		|| !collectible->collected)
		return (false);
	return (true);
}

static void	store_collectible_positions(t_collectible *collectible, t_map *map)
{
	int	i;
	int	j;
	int	count;

	count = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == 'C')
			{
				collectible->x_positions[count] = j * SPRITE_SIZE;
				collectible->y_positions[count] = i * SPRITE_SIZE;
				collectible->collected[count] = 0;
				count++;
			}
		}
	}
}

static void	load_collectible_sprites(t_collectible *collectible)
{
	collectible->base.sprites[0] = create_sprite("assets/collect/col00.xpm");
	collectible->base.sprites[1] = create_sprite("assets/collect/col01.xpm");
	collectible->base.sprites[2] = create_sprite("assets/collect/col02.xpm");
	collectible->base.sprites[3] = create_sprite("assets/collect/col03.xpm");
	collectible->base.sprites[4] = create_sprite("assets/misc/ban_col.xpm");
	init_animated_sprite(&collectible->base, 4, COLLECTIBLE_FRAME_DELAY);
}

void	init_collectible(void)
{
	t_collectible	*collectible;
	t_map			*map;
	int				count;

	collectible = &get_game()->collectible;
	map = &get_game()->map;
	count = count_collectible_positions(map);
	collectible->count = count;
	if (!allocate_collectible_arrays(collectible, count))
		exit_error(ERR_MEMORY);
	store_collectible_positions(collectible, map);
	load_collectible_sprites(collectible);
	get_game()->collectible_count = 0;
}
