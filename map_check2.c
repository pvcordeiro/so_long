#include "so_long.h"

static void count_entities(char **map, t_game *game)
{
    int i;
    int j;

    game->num_walls = 0;
    game->num_collectibles = 0;
    game->num_enemies = 0;

    i = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            if (map[i][j] == '1')
                game->num_walls++;
            else if (map[i][j] == 'C')
                game->num_collectibles++;
            else if (map[i][j] == 'e')
                game->num_enemies++;
            else if (map[i][j] == 'P')
            {
                game->player.x = j * TILE_SIZE;
                game->player.y = i * TILE_SIZE;
            }
            j++;
        }
        i++;
    }
}

void parse_map(char **map)
{
    t_game *game;
    int i;
    int j;
    int wall_index;
    int collectible_index;
    int enemy_index;

    game = get_game();
    count_entities(map, game);

    game->walls = malloc(sizeof(t_animation) * game->num_walls);
    game->collectibles = malloc(sizeof(t_animation) * game->num_collectibles);
    game->enemies = malloc(sizeof(t_animation) * game->num_enemies);

    wall_index = 0;
    collectible_index = 0;
    enemy_index = 0;

    i = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            if (map[i][j] == '1')
            {
                game->walls[wall_index].x = j * TILE_SIZE;
                game->walls[wall_index].y = i * TILE_SIZE;
                wall_index++;
            }
            else if (map[i][j] == 'C')
            {
                game->collectibles[collectible_index].x = j * TILE_SIZE;
                game->collectibles[collectible_index].y = i * TILE_SIZE;
                collectible_index++;
            }
            else if (map[i][j] == 'e')
            {
                game->enemies[enemy_index].x = j * TILE_SIZE;
                game->enemies[enemy_index].y = i * TILE_SIZE;
                enemy_index++;
            }
            j++;
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    char **map;
    int height;

    if (argc != 2)
    {
        ft_printf("Error\nUsage: ./so_long [map.ber]\n");
        return (1);
    }

    map = read_map(argv[1], &height);
    if (!map)
    {
        ft_printf("Error\nInvalid map\n");
        return (1);
    }

    init_window();
    parse_map(map);
    init_sprites();
    init_entities();
    setup_hooks();

    // Free map after parsing
    while (height--)
        free(map[height]);
    free(map);

    mlx_loop(get_game()->mlx);
    return (0);
}