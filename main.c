/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:14:46 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/17 22:49:35 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

t_game	*get_game(void)
{
	static t_game	game;

	return (&game);
}
unsigned int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

static int	check_collision(int x1, int y1, int x2, int y2, unsigned int width,
		unsigned int height)
{
	return (ft_abs(x1 - x2) < width &&
			ft_abs((y1 + COLLISION_Y_OFFSET)
			- y2) < height);
}

static int	check_enemy_collisions(int x, int y, int current_enemy)
{
	t_enemy_list	*enemy_list;
	int				i;

	enemy_list = &get_game()->enemy_list;
	i = -1;
	while (++i < enemy_list->count)
	{
		if (i != current_enemy && !enemy_list->enemies[i].is_dead
			&& check_collision(x, y, enemy_list->enemies[i].x + HITBOX_X_OFFSET,
				enemy_list->enemies[i].y + HITBOX_Y_OFFSET, ENEMY_COLLISION_WIDTH,
				ENEMY_COLLISION_HEIGHT))
			return (1);
	}
	return (0);
}


static void check_attack_collision(void)
{
    t_player *player;
    t_enemy_list *enemy_list;
	int	i;

    player = &get_game()->player;
    enemy_list = &get_game()->enemy_list;
	i = -1;
	while (++i < enemy_list->count)
	{
		if (!enemy_list->enemies[i].is_dead && player->is_attacking && enemy_list->enemies[i].invincibility_frames == 0)
		{
			if ((player->state == ATTACK_RIGHT && enemy_list->enemies[i].x > player->x) || (player->state == ATTACK_LEFT && enemy_list->enemies[i].x < player->x))
			{
				if (check_collision(player->x, player->y, enemy_list->enemies[i].x, enemy_list->enemies[i].y, ATTACK_RANGE, ENEMY_COLLISION_HEIGHT))
				{
					enemy_list->enemies[i].lives--;
					enemy_list->enemies[i].invincibility_frames = INVINCIBILITY_DURATION;
					if (player->x < enemy_list->enemies[i].x)
					{
						enemy_list->enemies[i].direction = 1;
						enemy_list->enemies[i].state = MOVE_RIGHT;
					}
					else
					{
						enemy_list->enemies[i].direction = -1;
						enemy_list->enemies[i].state = MOVE_LEFT;
					}
					if (enemy_list->enemies[i].lives <= 0)
						enemy_list->enemies[i].is_dead = true;
				}
			}
		}
	}
}

unsigned int	*get_pixel(t_img *data, int x, int y)
{
	return ((unsigned int *)(data->addr + (y * data->line_len + x * (data->bpp
					/ 8))));
}

static void	cleanup_sprites(void)
{
	t_game	*game;
	int		i;
	int		j;

	game = get_game();
	free(game->wall.x_positions);
	free(game->wall.y_positions);
	free(game->collectible.x_positions);
	free(game->collectible.y_positions);
	free(game->collectible.collected);
	i = -1;
	while (++i < game->enemy_list.count)
	{
		j = -1;
		while (++j < 2)
		{
			mlx_destroy_image(game->mlx, game->enemy_list.enemies[i].move_right.sprites[j].img);
			mlx_destroy_image(game->mlx, game->enemy_list.enemies[i].move_left.sprites[j].img);
		}
	}
	free(game->enemy_list.enemies);
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
	mlx_destroy_image(game->mlx, game->canvas.img);
	mlx_destroy_image(game->mlx, game->floor.img);
	mlx_destroy_image(game->mlx, game->floor2.img);
	mlx_destroy_image(game->mlx, game->mushroom.sprite.img);
	mlx_destroy_image(game->mlx, game->exit.sprite.img);
    mlx_destroy_image(game->mlx, game->health.health1.img);
    mlx_destroy_image(game->mlx, game->health.health2.img);
    mlx_destroy_image(game->mlx, game->health.health3.img);
	mlx_destroy_image(game->mlx, game->health.sprint.img);
	mlx_destroy_image(game->mlx, game->health.banner.img);
	mlx_destroy_image(game->mlx, game->health.message.img);
}

int	exit_game(void)
{
	cleanup_sprites();
	mlx_destroy_window(get_game()->mlx, get_game()->win);
	mlx_destroy_display(get_game()->mlx);
	free(get_game()->mlx);
	ft_printf("Game closed\n");
	exit(EXIT_SUCCESS);
}
int	exit_error(void)
{
	cleanup_sprites();
	mlx_destroy_window(get_game()->mlx, get_game()->win);
	mlx_destroy_display(get_game()->mlx);
	free(get_game()->mlx);
	ft_printf("Error\n");
	exit(EXIT_FAILURE);
}

t_img	make_sprite(char *path)
{
	t_img	sprite;

	if (path)
	{
		sprite.img = mlx_xpm_file_to_image(get_game()->mlx, path, &sprite.width,
				&sprite.height);
		if (!sprite.img)
			exit_error();
	}
	else
		sprite.img = mlx_new_image(get_game()->mlx, get_game()->window_width,
				get_game()->window_height);
	sprite.x = 0;
	sprite.y = 0;
	sprite.addr = mlx_get_data_addr(sprite.img, &sprite.bpp, &sprite.line_len,
			&sprite.endian);
	return (sprite);
}

void	draw_image(t_img *src, t_img *dst, int x, int y)
{
	int	sx;
	int	sy;

	sy = 0;
	while (sy < src->height)
	{
		sx = 0;
		while (sx < src->width)
		{
			if (!(sx + x > get_game()->window_width || sy + y > get_game()->window_height || sx + x < 0
					|| sy + y < 0) && (*get_pixel(src, sx, sy) != 0xFF000000))
				*get_pixel(dst, x + sx, y + sy) = *get_pixel(src, sx, sy);
			sx++;
		}
		sy++;
	}
}

void	init_animation(t_animation *anim, int frame_count, int speed)
{
	anim->current_frame = 0;
	anim->anim_counter = 0;
	anim->anim_speed = speed;
	anim->frame_count = frame_count;
}

void	update_animation(t_animation *anim)
{
	anim->anim_counter++;
	if (anim->anim_counter >= anim->anim_speed)
	{
		anim->anim_counter = 0;
		anim->current_frame = (anim->current_frame + 1) % anim->frame_count;
	}
}

void	draw_animation(t_animation *anim, t_img *canvas)
{
	draw_image(&anim->sprites[anim->current_frame], canvas, anim->x, anim->y);
}

static void	update_collectible(void)
{
	t_collectible	*collectible;
	t_player		*player;
	int				i;

	collectible = &get_game()->collectible;
	player = &get_game()->player;
	i = -1;
	while (++i < collectible->count)
	{
		if (!collectible->collected[i] && check_collision(player->x, player->y,
            collectible->x_positions[i] + HITBOX_X_OFFSET, collectible->y_positions[i] + HITBOX_Y_OFFSET,
            COLLECTIBLE_SIZE, COLLECTIBLE_SIZE))
		{
			collectible->collected[i] = 1;
			get_game()->collectible_count++;
		}
	}
	update_animation(&collectible->base);
}

static void	init_collectible(void)
{
	t_collectible	*collectible;
	t_map			*map;
	int				i;
	int				j;
	int				count;

	collectible = &get_game()->collectible;
	map = &get_game()->map;
	count = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == 'C')
				count++;
		}
	}
	collectible->count = count;
	collectible->x_positions = malloc(sizeof(int) * count);
	collectible->y_positions = malloc(sizeof(int) * count);
	collectible->collected = malloc(sizeof(int) * count);
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
	collectible->base.sprites[0] = make_sprite("assets/collect/collect00.xpm");
	collectible->base.sprites[1] = make_sprite("assets/collect/collect01.xpm");
	collectible->base.sprites[2] = make_sprite("assets/collect/collect02.xpm");
	collectible->base.sprites[3] = make_sprite("assets/collect/collect03.xpm");
	collectible->base.sprites[4] = make_sprite("assets/ban_col.xpm");
	init_animation(&collectible->base, 4, COLLECTIBLE_ANIMATION_SPEED);
	get_game()->collectible_count = 0;
}

static void	init_wall(void)
{
	t_wall	*wall;
	int	i;
	int	j;
	int	count;
	t_map *map;

	wall = &get_game()->wall;
	map = &get_game()->map;
	count = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == '1')
				count++;
		}
	}
	wall->count = count;
	wall->x_positions = malloc(sizeof(int) * count);
	wall->y_positions = malloc(sizeof(int) * count);
	count = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == '1')
			{
				wall->x_positions[count] = j * SPRITE_SIZE;
				wall->y_positions[count] = i * SPRITE_SIZE;
				count++;
			}
		}
	}
	wall->base.sprites[0] = make_sprite("assets/terrain/lava00.xpm");
	wall->base.sprites[1] = make_sprite("assets/terrain/lava01.xpm");
	init_animation(&wall->base, FRAME_COUNT, WALL_ANIMATION_SPEED);
}

static int check_wall_collisions(int x, int y, unsigned int width, unsigned int height)
{
	t_wall	*wall;
	int		i;

	wall = &get_game()->wall;
	i = -1;
	while (++i < wall->count)
	{
		if (check_collision(x, y, wall->x_positions[i], wall->y_positions[i],
				width, height))
			return (1);
	}
	return (0);
}

static void	init_enemy(void)
{
	t_enemy_list	*enemy_list;
	t_map			*map;
	int				i;
	int 		   	j;
	int				count;

	enemy_list = &get_game()->enemy_list;
	map = &get_game()->map;
	count = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == 'X')
				count++;
		}
	}
	enemy_list->count = count;
	enemy_list->enemies = malloc(sizeof(t_enemy) * count);
	count = 0;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == 'X')
			{
				enemy_list->enemies[count].move_right.sprites[0] = make_sprite("assets/enemy/move_right/enemy_move_right00.xpm");
				enemy_list->enemies[count].move_right.sprites[1] = make_sprite("assets/enemy/move_right/enemy_move_right01.xpm");
				init_animation(&enemy_list->enemies[count].move_right, FRAME_COUNT, ENEMY_ANIMATION_SPEED);
				enemy_list->enemies[count].move_left.sprites[0] = make_sprite("assets/enemy/move_left/enemy_move_left00.xpm");
				enemy_list->enemies[count].move_left.sprites[1] = make_sprite("assets/enemy/move_left/enemy_move_left01.xpm");
				init_animation(&enemy_list->enemies[count].move_left, FRAME_COUNT, ENEMY_ANIMATION_SPEED);
				enemy_list->enemies[count].x = j * SPRITE_SIZE;
                enemy_list->enemies[count].y = i * SPRITE_SIZE;
                enemy_list->enemies[count].direction = (rand() % 2) * 2 - 1;
                enemy_list->enemies[count].y_direction = 0;
                enemy_list->enemies[count].state = (enemy_list->enemies[count].direction == 1) ? MOVE_RIGHT : MOVE_LEFT;
                enemy_list->enemies[count].move_counter = 0;
                enemy_list->enemies[count].lives = 3;
                enemy_list->enemies[count].is_dead = false;
                enemy_list->enemies[count].invincibility_frames = 0;
                enemy_list->enemies[count].is_visible = true;
                count++;
			}
		}
	}
}

static void update_enemy(void)
{
    t_enemy_list *enemy_list;
    t_animation *current_anim;
    int new_x;
    int new_y;
	int	prev_x;
	int	prev_y;
    int i;

    enemy_list = &get_game()->enemy_list;
    i = -1;
    while (++i < enemy_list->count)
    {
        if (enemy_list->enemies[i].is_dead)
            continue;
        if (enemy_list->enemies[i].invincibility_frames > 0)
            enemy_list->enemies[i].invincibility_frames--;

        enemy_list->enemies[i].move_counter++;
        if (enemy_list->enemies[i].move_counter >= ENEMY_MOVE_THRESHOLD)
        {
            if (rand() % 2 == 0)
            {
                enemy_list->enemies[i].direction = (rand() % 2) * 2 - 1;
                enemy_list->enemies[i].y_direction = 0;
            }
            else
            {
                enemy_list->enemies[i].direction = 0;
                enemy_list->enemies[i].y_direction = (rand() % 2) * 2 - 1;
            }
            enemy_list->enemies[i].move_counter = 0;
        }
        prev_x = enemy_list->enemies[i].x;
        prev_y = enemy_list->enemies[i].y;
        new_x = prev_x + enemy_list->enemies[i].direction * ENEMY_SPEED;
        enemy_list->enemies[i].x = new_x;
        if (check_wall_collisions(new_x, prev_y, ENEMY_WALL_COLLISION_WIDTH, ENEMY_WALL_COLLISION_HEIGHT) 
            || check_enemy_collisions(new_x, prev_y, i))
        {
            enemy_list->enemies[i].x = prev_x;
            enemy_list->enemies[i].direction *= -1;
        }
        new_y = prev_y + enemy_list->enemies[i].y_direction * ENEMY_SPEED;
        enemy_list->enemies[i].y = new_y;
        if (check_wall_collisions(enemy_list->enemies[i].x, new_y, ENEMY_WALL_COLLISION_WIDTH, ENEMY_WALL_COLLISION_HEIGHT) 
            || check_enemy_collisions(enemy_list->enemies[i].x, new_y, i))
        {
            enemy_list->enemies[i].y = prev_y;
            enemy_list->enemies[i].y_direction *= -1;
        }
        if (enemy_list->enemies[i].direction > 0)
            enemy_list->enemies[i].state = MOVE_RIGHT;
        else if (enemy_list->enemies[i].direction < 0)
            enemy_list->enemies[i].state = MOVE_LEFT;
        if (enemy_list->enemies[i].state == MOVE_RIGHT)
            current_anim = &enemy_list->enemies[i].move_right;
        else
            current_anim = &enemy_list->enemies[i].move_left;
        update_animation(current_anim);
    }
}

static void draw_enemy(void)
{
    t_enemy_list *enemy_list;
    t_animation *current_anim;
    int i;

    enemy_list = &get_game()->enemy_list;
    i = -1;
    while (++i < enemy_list->count)
    {
        if (enemy_list->enemies[i].is_dead)
            continue;
        if (enemy_list->enemies[i].invincibility_frames > 0)
        {
            enemy_list->enemies[i].is_visible = !enemy_list->enemies[i].is_visible;
            if (!enemy_list->enemies[i].is_visible)
                continue;
        }

        if (enemy_list->enemies[i].state == MOVE_RIGHT)
            current_anim = &enemy_list->enemies[i].move_right;
        else
            current_anim = &enemy_list->enemies[i].move_left;

        current_anim->x = enemy_list->enemies[i].x;
        current_anim->y = enemy_list->enemies[i].y;
        draw_animation(current_anim, &get_game()->canvas);
    }
}

void	draw_floor(void)
{
	int	x;
	int	y;

	y = -1;
	while (++y <= get_game()->window_height / SPRITE_SIZE)
	{
		x = -1;
		while (++x <= (get_game()->window_width / SPRITE_SIZE))
		{
			if ((x + y) % 2 == 0)
				draw_image(&get_game()->floor, &get_game()->canvas, x * SPRITE_SIZE, y
					* SPRITE_SIZE);
			else
				draw_image(&get_game()->floor2, &get_game()->canvas, x * SPRITE_SIZE, y
					* SPRITE_SIZE);
		}
	}
}

static void draw_collectibles(void)
{
	t_collectible	*collectible;
	int				i;

	collectible = &get_game()->collectible;
	i = -1;
	while (++i < collectible->count)
	{
		if (!collectible->collected[i])
		{
			collectible->base.x = collectible->x_positions[i];
			collectible->base.y = collectible->y_positions[i];
			draw_animation(&collectible->base, &get_game()->canvas);
		}
	}
}

static void	draw_walls(void)
{
	t_wall	*wall;
	int		i;

	wall = &get_game()->wall;
	update_animation(&wall->base);

	i = -1;
	while (++i < wall->count)
	{
		wall->base.x = wall->x_positions[i];
		wall->base.y = wall->y_positions[i];
		draw_animation(&wall->base, &get_game()->canvas);
	}
}

int	key_loop(int key, char *action)
{
	t_player	*player;

	player = &get_game()->player;
	if (key == XK_Escape)
		exit_game();
	if (!get_game()->vic && !get_game()->game_over)
	{
		if ( key == XK_Shift_L)
		{
			if (*action == 'p' && get_game()->player.can_sprint)
				get_game()->player.is_sprinting = true;
			else if (*action == 'r')
				get_game()->player.is_sprinting = false;
		}
		if (key == XK_w)
			get_game()->move_up = (*action == 'p');
		if (key == XK_a)
			get_game()->move_left = (*action == 'p');
		if (key == XK_s)
			get_game()->move_down = (*action == 'p');
		if (key == XK_d)
			get_game()->move_right = (*action == 'p');
		if (key == XK_space && *action == 'p' && !player->is_attacking && player->attack_cooldown == 0)
		{
			player->is_attacking = true;
			player->attack_frame = 0;
			if (player->state == IDLE_RIGHT || player->state == MOVE_RIGHT)
			{
				player->state = ATTACK_RIGHT;
				player->attack_right.current_frame = 0;
			}
			else
			{
				player->state = ATTACK_LEFT;
				player->attack_left.current_frame = 0;
			}
		}
	}
	return (0);
}

static void	init_exit(void)
{
	t_exit	*exit;
	t_map	*map;
	int		i;
	int		j;

	exit = &get_game()->exit;
	map = &get_game()->map;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == 'E')
			{
				exit->x = j * SPRITE_SIZE;
				exit->y = i * SPRITE_SIZE;
				break ;
			}
		}
	}
	exit->sprite = make_sprite("assets/exit.xpm");
}

static void draw_exit_bottom(void)
{
    t_exit *exit;
    t_img temp;
    int i;
    int j;
    int draw_y;

    exit = &get_game()->exit;
    temp = exit->sprite;
    draw_y = exit->y - (exit->sprite.height - SPRITE_SIZE);
    
    // Only draw bottom half
    for (i = temp.height/2; i < temp.height; i++)
    {
        for (j = 0; j < temp.width; j++)
        {
            if (*get_pixel(&temp, j, i) != 0xFF000000)
                *get_pixel(&get_game()->canvas, 
                    exit->x + j, 
                    draw_y + i) = *get_pixel(&temp, j, i);
        }
    }
}

static void draw_exit_top(void)
{
    t_exit *exit;
    t_img temp;
    int i;
    int j;
    int draw_y;

    exit = &get_game()->exit;
    temp = exit->sprite;
    draw_y = exit->y - (exit->sprite.height - SPRITE_SIZE);
    
    // Only draw top half
    for (i = 0; i < temp.height/2; i++)
    {
        for (j = 0; j < temp.width; j++)
        {
            if (*get_pixel(&temp, j, i) != 0xFF000000)
                *get_pixel(&get_game()->canvas, 
                    exit->x + j, 
                    draw_y + i) = *get_pixel(&temp, j, i);
        }
    }
}
static void	draw_exit_full(void)
{
	t_exit	*exit;
	int		draw_y;

	exit = &get_game()->exit;
	draw_y = exit->y - (exit->sprite.height - SPRITE_SIZE);
	draw_image(&exit->sprite, &get_game()->canvas, exit->x, draw_y);
}

static void	init_health(void)
{
	t_health	*health;

	health = &get_game()->health;
	health->health1 = make_sprite("assets/health/health1.xpm");
	health->health2 = make_sprite("assets/health/health2.xpm");
	health->health3 = make_sprite("assets/health/health3.xpm");
	health->sprint = make_sprite("assets/sprint.xpm");
	health->banner = make_sprite("assets/banner.xpm");
	health->message = make_sprite("assets/helper_message.xpm");
}

static void	draw_health(void)
{
	t_health	*health;
	t_img		*current_sprite;

	health = &get_game()->health;
	if (get_game()->player.lives == 3)
		current_sprite = &health->health3;
	else if (get_game()->player.lives == 2)
		current_sprite = &health->health2;
	else if (get_game()->player.lives == 1)
		current_sprite = &health->health1;
	else
		return ;
	draw_image(current_sprite, &get_game()->canvas, 10, -10);
}

static void init_mushroom(void)
{
    t_mushroom *mushroom;
    t_map *map;
    int valid_positions;
    int random_position;
    int current_position;
    int i;
	int j;

    mushroom = &get_game()->mushroom;
    map = &get_game()->map;
    mushroom->sprite = make_sprite("assets/mushroom.xpm");
    mushroom->active = true;
    mushroom->collected = false;
    valid_positions = 0;
    i = -1;
    while (++i < map->height)
    {
        j = -1;
        while (++j < map->width)
        {
            if (map->map[i][j] == '0')
                valid_positions++;
        }
    }
    random_position = rand() % valid_positions;
    current_position = 0;
    i = -1;
    while (++i < map->height)
    {
        j = -1;
        while (++j < map->width)
        {
            if (map->map[i][j] == '0')
            {
                if (current_position == random_position)
                {
                    mushroom->x = j * SPRITE_SIZE;
                    mushroom->y = i * SPRITE_SIZE;
                    return;
                }
                current_position++;
            }
        }
    }
}

static void update_mushroom(void)
{
    t_mushroom *mushroom;
    t_player *player;

    mushroom = &get_game()->mushroom;
    player = &get_game()->player;

    if (!mushroom->active || mushroom->collected)
        return;

    if (check_collision(player->x, player->y,
        mushroom->x + HITBOX_X_OFFSET, mushroom->y + HITBOX_Y_OFFSET, COLLECTIBLE_SIZE, COLLECTIBLE_SIZE))
    {
        if (player->lives < 3)
        {
            player->lives++;
            mushroom->collected = true;
            mushroom->active = false;
        }
    }
}

static void draw_mushroom(void)
{
    t_mushroom *mushroom;

    mushroom = &get_game()->mushroom;
    if (mushroom->active && !mushroom->collected)
    {
        draw_image(&mushroom->sprite, &get_game()->canvas,
            mushroom->x, mushroom->y);
    }
}

static void	init_player(void)
{
	t_player	*player;
	t_map		*map;
	int			i;
	int			j;

	player = &get_game()->player;
	map = &get_game()->map;
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			if (map->map[i][j] == 'P')
			{
				player->x = j * SPRITE_SIZE;
				player->y = i * SPRITE_SIZE;
				break ;
			}
		}
	}
	get_game()->move_count = 0;
	player->idle_right.sprites[0] = make_sprite("assets/player/idle_right/player_idle_right00.xpm");
	player->idle_right.sprites[1] = make_sprite("assets/player/idle_right/player_idle_right01.xpm");
	init_animation(&player->idle_right, FRAME_COUNT, PLAYER_IDLE_ANIMATION_SPEED);
	player->idle_left.sprites[0] = make_sprite("assets/player/idle_left/player_idle_left00.xpm");
	player->idle_left.sprites[1] = make_sprite("assets/player/idle_left/player_idle_left01.xpm");
	init_animation(&player->idle_left, FRAME_COUNT, PLAYER_IDLE_ANIMATION_SPEED);
	player->move_right.sprites[0] = make_sprite("assets/player/move_right/player_move_right00.xpm");
	player->move_right.sprites[1] = make_sprite("assets/player/move_right/player_move_right01.xpm");
	init_animation(&player->move_right, FRAME_COUNT, PLAYER_MOVE_AND_ATTACK_ANIMATION_SPEED);
	player->move_left.sprites[0] = make_sprite("assets/player/move_left/player_move_left00.xpm");
	player->move_left.sprites[1] = make_sprite("assets/player/move_left/player_move_left01.xpm");
	init_animation(&player->move_left, FRAME_COUNT, PLAYER_MOVE_AND_ATTACK_ANIMATION_SPEED);
	player->attack_right.sprites[0] = make_sprite("assets/player/attack_right/player_attack_right00.xpm");
	player->attack_right.sprites[1] = make_sprite("assets/player/attack_right/player_attack_right01.xpm");
	init_animation(&player->attack_right, FRAME_COUNT, PLAYER_MOVE_AND_ATTACK_ANIMATION_SPEED);
	player->attack_left.sprites[0] = make_sprite("assets/player/attack_left/player_attack_left00.xpm");
	player->attack_left.sprites[1] = make_sprite("assets/player/attack_left/player_attack_left01.xpm");
	init_animation(&player->attack_left, FRAME_COUNT, PLAYER_MOVE_AND_ATTACK_ANIMATION_SPEED);
	player->state = IDLE_RIGHT;
	player->last_direction = MOVE_RIGHT;
	player->lives = 3;
	player->invincibility_frames = 0;
	player->is_visible = 1;
	player->attack_cooldown = 0;
	player->max_attack_cooldown = ATTACK_COOLDOWN;
	player->is_attacking = false;
	player->attack_frame = 0;
	player->is_sprinting = false;
	player->sprint_duration = 0;
	player->sprint_cooldown = 0;
	player->can_sprint = true;
}

static void	update_player_position(void)
{
	int			prev_x;
	int			prev_y;
	t_player	*player;
	static int	distance_moved = 0;
	int			movement_speed;

	player = &get_game()->player;
	prev_x = player->x;
	prev_y = player->y;
	movement_speed = PLAYER_SPEED;
	if (player->is_sprinting)
	{
		if (player->sprint_duration < SPRINT_DURATION)
		{
			movement_speed *= SPRINT_MULTIPLIER;
			player->sprint_duration++;
		}
		else
		{
			player->is_sprinting = false;
			player->can_sprint = false;
			player->sprint_duration = 0;
		}
	}
	if (!player->can_sprint)
	{
		player->sprint_cooldown++;
		if (player->sprint_cooldown >= SPRINT_COOLDOWN)
		{
			player->can_sprint = true;
			player->sprint_cooldown = 0;
		}
	}
    if (!player->is_attacking)
    {
        if (get_game()->move_left)
            player->x -= movement_speed;
        if (get_game()->move_right)
            player->x += movement_speed;
        if (check_wall_collisions(player->x, player->y, WALL_COLLISION_WIDTH, WALL_COLLISION_HEIGHT))
            player->x = prev_x;
        if (get_game()->move_up)
            player->y -= movement_speed;
        if (get_game()->move_down)
            player->y += movement_speed;
        if (check_wall_collisions(player->x, player->y, WALL_COLLISION_WIDTH, WALL_COLLISION_HEIGHT))
            player->y = prev_y;
    }
	if (prev_x != player->x || prev_y != player->y)
    {
        distance_moved += abs(player->x - prev_x) + abs(player->y - prev_y);
        if (distance_moved >= SPRITE_SIZE)
        {
            get_game()->move_count++;
            distance_moved = 0;
        }
    }
}

static void	update_player(void)
{
	t_player	*player;
	t_animation	*current_anim;

	player = &get_game()->player;

	if (player->attack_cooldown > 0)
		player->attack_cooldown--;
	if (player->is_attacking)
	{
		player->attack_timer++;
		if (player->state == ATTACK_RIGHT)
			current_anim = &player->attack_right;
		else
			current_anim = &player->attack_left;
		if (player->attack_timer < 10)
			current_anim->current_frame = 0;
		else if (player->attack_timer < 20)
			current_anim->current_frame = 1;
		else
		{
			player->is_attacking = false;
			player->attack_cooldown = player->max_attack_cooldown;
			player->attack_timer = 0;
			if (player->state == ATTACK_RIGHT)
				player->state = IDLE_RIGHT;
			else
				player->state = IDLE_LEFT;
		}
		return;
	}
	if (get_game()->move_right)
	{
		player->state = MOVE_RIGHT;
		player->last_direction = MOVE_RIGHT;
	}
	else if (get_game()->move_left)
	{
		player->state = MOVE_LEFT;
		player->last_direction = MOVE_LEFT;
	}
	else if (get_game()->move_up || get_game()->move_down)
	{
		if (player->last_direction == MOVE_RIGHT)
			player->state = MOVE_RIGHT;
		else
			player->state = MOVE_LEFT;
	}
	else
	{
		if (player->last_direction == MOVE_RIGHT)
			player->state = IDLE_RIGHT;
		else
			player->state = IDLE_LEFT;
	}
	if (player->state == IDLE_RIGHT)
		current_anim = &player->idle_right;
	else if (player->state == IDLE_LEFT)
		current_anim = &player->idle_left;
	else if (player->state == MOVE_RIGHT)
		current_anim = &player->move_right;
	else
		current_anim = &player->move_left;
	update_animation(current_anim);
}

static void	draw_player(void)
{
	t_player	*player;
	t_animation	*current_anim;

	player = &get_game()->player;
	current_anim = NULL;
	if (player->invincibility_frames > 0)
	{
		player->is_visible = !player->is_visible;
		if (!player->is_visible)
			return ;
	}
	if (player->state == IDLE_RIGHT)
		current_anim = &player->idle_right;
	else if (player->state == IDLE_LEFT)
		current_anim = &player->idle_left;
	else if (player->state == MOVE_RIGHT)
		current_anim = &player->move_right;
	else if (player->state == MOVE_LEFT)
		current_anim = &player->move_left;
	else if (player->state == ATTACK_RIGHT)
		current_anim = &player->attack_right;
	else if (player->state == ATTACK_LEFT)
		current_anim = &player->attack_left;
	current_anim->x = player->x;
	current_anim->y = player->y;
	draw_animation(current_anim, &get_game()->canvas);
}

static void	fps_cap(void)
{
	static struct timeval	last_frame = {0, 0};
	struct timeval			current_time;
	long					elapsed_us;

	gettimeofday(&current_time, NULL);
	elapsed_us = (current_time.tv_sec - last_frame.tv_sec) * 1000000
		+ (current_time.tv_usec - last_frame.tv_usec);
	if (elapsed_us < FRAME_DELAY)
		usleep(FRAME_DELAY - elapsed_us);
	last_frame = current_time;
}

static void handle_game_state(void)
{
    t_player *player;
    t_enemy_list *enemy_list;
    int i;

    player = &get_game()->player;
    enemy_list = &get_game()->enemy_list;

    if (player->invincibility_frames > 0)
        player->invincibility_frames--;

    i = -1;
    while (++i < enemy_list->count)
    {
        if (!enemy_list->enemies[i].is_dead && 
            check_collision(player->x, player->y,
                enemy_list->enemies[i].x, enemy_list->enemies[i].y,
                ENEMY_COLLISION_WIDTH, ENEMY_COLLISION_HEIGHT) && 
            player->invincibility_frames == 0)
        {
            player->lives--;
            player->invincibility_frames = INVINCIBILITY_DURATION;
            if (player->lives <= 0)
            {
                get_game()->game_over = true;
            }
            break;
        }
    }
}

void helper_message(void)
{
    if (check_collision(get_game()->player.x, get_game()->player.y,
        get_game()->exit.x, get_game()->exit.y, 40, 40) && 
        get_game()->collectible_count < get_game()->collectible.count)
    {
        int x = get_game()->exit.x + (SPRITE_SIZE - 160) / 2;
        int y = get_game()->exit.y - 40;
        
        draw_image(&get_game()->health.message, &get_game()->canvas, x, y);
    }
}

static void print_moves(void)
{
    char *print_move;
    int text_x;
    int text_y;
    
    text_x = 25;
    text_y = get_game()->window_height - 35;
    print_move = ft_itoa(get_game()->move_count);
    mlx_string_put(get_game()->mlx, get_game()->win, text_x, text_y - 10,
        0x000000, "MOVES");
    mlx_string_put(get_game()->mlx, get_game()->win, text_x + 8, text_y + 5,
        0x000000, print_move);
    free(print_move);
}

void victory_check(void)
{
	if (get_game()->collectible_count == get_game()->collectible.count
		&& check_collision(get_game()->player.x, get_game()->player.y,
			get_game()->exit.x, get_game()->exit.y, 20, 20))
	{
		get_game()->vic = true;
	}
}

static void draw_collectible_counter(void)
{
    t_collectible *collectible;
    char *count_str;
    int x;
    int y;

    x = get_game()->window_width - 80;
    y = 0;
    collectible = &get_game()->collectible;
    draw_image(&collectible->base.sprites[4], &get_game()->canvas, x, y);
    count_str = ft_itoa(get_game()->collectible_count);
    mlx_string_put(get_game()->mlx, get_game()->win, 
        x + 46, y + 45, 0x000000, count_str);
    free(count_str);
}

static void draw_sprint_icon(void)
{
    t_player *player;
    static int flash_counter = 0;
    int sprint_x;
    int sprint_y;

    player = &get_game()->player;
    sprint_x = get_game()->window_width - 80;
    sprint_y = get_game()->window_height - 80;
    if (player->is_sprinting)
        draw_image(&get_game()->health.sprint, &get_game()->canvas, sprint_x, sprint_y);
    else if (!player->can_sprint)
    {
        flash_counter++;
        if ((flash_counter / 15) % 2)
            draw_image(&get_game()->health.sprint, &get_game()->canvas, sprint_x, sprint_y);
    }
    else
        draw_image(&get_game()->health.sprint, &get_game()->canvas, sprint_x, sprint_y);
}

static void draw_ui_banners(void)
{
    int banner_top_right;
    int banner_bottom_left;
    int banner_bottom_right;
    
    banner_top_right = get_game()->window_width - 80;
    draw_image(&get_game()->health.banner, &get_game()->canvas, 
        banner_top_right, 0);
    banner_bottom_left = 0;
    draw_image(&get_game()->health.banner, &get_game()->canvas, 
        banner_bottom_left, get_game()->window_height - 80);
    banner_bottom_right = get_game()->window_width - 80;
    draw_image(&get_game()->health.banner, &get_game()->canvas, 
        banner_bottom_right, get_game()->window_height - 80);
}

static void draw_end_game_screen(void)
{
    t_game *game;
    int x;
    int y;

    game = get_game();
    if (game->vic || game->game_over)
    {
        x = (game->window_width - 400) / 2;
        y = (game->window_height - 400) / 2;
        
        if (game->vic)
            draw_image(&game->victory, &game->canvas, x, y);
        else
            draw_image(&game->defeat, &game->canvas, x, y);
    }
}

int game_loop(void)
{
    mlx_put_image_to_window(get_game()->mlx, get_game()->win,
        get_game()->canvas.img, 0, 0);
    fps_cap();
    draw_floor();
    draw_walls();
    update_animation(&get_game()->wall.base);
    draw_animation(&get_game()->wall.base, &get_game()->canvas);
    draw_end_game_screen();
    if (!get_game()->vic && !get_game()->game_over)
    {
        handle_game_state();
        update_mushroom();
        update_collectible();
        update_enemy();
        check_attack_collision();
        update_player_position();
        update_player();
        draw_mushroom();
        draw_collectibles();
        draw_enemy();
        draw_health();
        victory_check();
        draw_ui_banners();
        draw_sprint_icon();
        if (get_game()->player.y < get_game()->exit.y)
        {
            draw_player();
            draw_exit_full();
        }
        else
        {
            draw_exit_bottom();
            draw_player();
            draw_exit_top();
        }
        helper_message();
        print_moves();
        draw_collectible_counter();
    }
    return (0);
}

static void	init_window(void)
{
	t_game	*game;

	game = get_game();
	game->window_width = game->map.width * SPRITE_SIZE;
	game->window_height = game->map.height * SPRITE_SIZE;
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, game->window_width,
			game->window_height, "so_long");
	game->game_over = false;
	game->vic = false;
	if (!game->mlx || !game->win)
		exit_error();
}

static void	init_sprites(void)
{
	get_game()->canvas = make_sprite(NULL);
	get_game()->floor = make_sprite("assets/terrain/floor00.xpm");
	get_game()->floor2 = make_sprite("assets/terrain/floor01.xpm");
	get_game()->victory = make_sprite("assets/victory.xpm");
	get_game()->defeat = make_sprite("assets/defeat.xpm");
	init_collectible();
	init_wall();
	init_player();
	init_enemy();
	init_exit();
	init_health();
	init_mushroom();
}

static void	setup_hooks(void)
{
	mlx_loop_hook(get_game()->mlx, game_loop, NULL);
	mlx_hook(get_game()->win, KeyPress, KeyPressMask, key_loop, "p");
	mlx_hook(get_game()->win, KeyRelease, KeyReleaseMask, key_loop, "r");
	mlx_hook(get_game()->win, DestroyNotify, KeyPressMask, exit_game, NULL);
}

static bool is_map_surrounded(t_map *map)
{
    int i;

    i = -1;
    while (++i < map->width)
        if (map->map[0][i] != '1' || map->map[map->height - 1][i] != '1')
            return (false);
    i = -1;
    while (++i < map->height)
        if (map->map[i][0] != '1' || map->map[i][map->width - 1] != '1')
            return (false);
    return (true);
}

static bool count_entities(t_map *map, int *player, int *exit, int *collect, int *empty)
{
    int i;
    int j;

    *player = 0;
    *exit = 0;
    *collect = 0;
	*empty = 0;
    i = -1;
    while (++i < map->height)
    {
        j = -1;
        while (++j < map->width)
        {
            if (map->map[i][j] == 'P')
                (*player)++;
            else if (map->map[i][j] == 'E')
                (*exit)++;
            else if (map->map[i][j] == 'C')
                (*collect)++;
			else if (map->map[i][j] == '0')
				(*empty)++;
        }
    }
    return (*player == 1 && *exit == 1 && *collect > 0 && *empty > 0);
}

static void flood_fill(char **map, int x, int y, int *collect)
{
    if (map[y][x] == '1' || map[y][x] == 'X')
        return;
    if (map[y][x] == 'C')
        (*collect)--;
    map[y][x] = 'X';
    flood_fill(map, x + 1, y, collect);
    flood_fill(map, x - 1, y, collect);
    flood_fill(map, x, y + 1, collect);
    flood_fill(map, x, y - 1, collect);
}

static bool check_path(t_map *map)
{
    char **temp_map;
    int i, j, collect, px, py;
    bool exit_found;

    temp_map = malloc(sizeof(char *) * map->height);
    i = -1;
    while (++i < map->height)
    {
        temp_map[i] = ft_strdup(map->map[i]);
        if (!temp_map[i])
        {
            while (--i >= 0)
                free(temp_map[i]);
            free(temp_map);
            return (false);
        }
    }
    px = py = -1;
    i = -1;
    while (++i < map->height)
    {
        j = -1;
        while (++j < map->width)
            if (map->map[i][j] == 'P')
            {
                px = j;
                py = i;
                break;
            }
    }
    collect = 0;
    i = -1;
    while (++i < map->height)
    {
        j = -1;
        while (++j < map->width)
            if (map->map[i][j] == 'C')
                collect++;
    }
    flood_fill(temp_map, px, py, &collect);
    exit_found = false;
    i = -1;
    while (++i < map->height)
    {
        j = -1;
        while (++j < map->width)
            if (map->map[i][j] == 'E' && temp_map[i][j] == 'X')
                exit_found = true;
    }
    i = -1;
    while (++i < map->height)
        free(temp_map[i]);
    free(temp_map);
    return (exit_found && collect == 0);
}

static bool validate_map(t_map *map)
{
    int player, exit, collect, empty;

    if (!is_map_surrounded(map))
        return (false);
    if (!count_entities(map, &player, &exit, &collect, &empty))
        return (false);
    if (!check_path(map))
        return (false);
    return (true);
}

t_map *parse_map(char *filename)
{
    t_map *map_info;
    char *line;
    int fd;
    int i;
    map_info = malloc(sizeof(t_map));
    if (!map_info)
        return (NULL);
    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        free(map_info);
        return (NULL);
    }
    map_info->height = 0;
    map_info->width = 0;
    while ((line = get_next_line(fd)) != NULL)
    {
        if (map_info->height == 0)
            map_info->width = ft_strlen(line) - 1;
        map_info->height++;
        free(line);
    }
    close(fd);
    map_info->map = malloc(sizeof(char *) * (map_info->height + 1));
    if (!map_info->map)
    {
        free(map_info);
        return (NULL);
    }
    fd = open(filename, O_RDONLY);
    i = 0;
    while ((line = get_next_line(fd)) != NULL)
    {
        map_info->map[i] = line;
        i++;
    }
    map_info->map[i] = NULL;
    close(fd);

    map_info->collectibles = 0;
    map_info->collectibles_reachable = 0;
    map_info->exit_reachable = 0;
	if (!validate_map(map_info))
	{
		i = -1;
		while (++i < map_info->height)
			free(map_info->map[i]);
		free(map_info->map);
		free(map_info);
		return (NULL);
	}

    return (map_info);
}

int main(int argc, char **argv)
{
    t_map *map_info;

    if (argc != 2)
        return (ft_printf("Error\nUsage: ./so_long [map.ber]\n"));
    map_info = parse_map(argv[1]);
    if (!map_info)
		return (ft_printf("Error\nInvalid map\n"));
	srand(time(NULL));
	get_game()->map = *map_info;
    init_window();
    init_sprites();
    setup_hooks();
    mlx_loop(get_game()->mlx);
    return (0);
}
