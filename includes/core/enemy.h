/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 03:10:10 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:44:51 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENEMY_H
# define ENEMY_H

# include "../types/common_types.h"
# include "../types/enemy_types.h"

void	init_enemy(void);
void	init_enemy_state(t_enemy *enemy, int x, int y);
void	init_enemy_animations(t_enemy *enemy);
void	update_enemy(void);
void	draw_enemy(void);
void	handle_enemy_collision(t_enemy *enemy, int prev_x, int prev_y);
int		check_enemy_collisions(int x, int y, int current_enemy);

#endif