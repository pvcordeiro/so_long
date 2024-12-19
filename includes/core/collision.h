/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 03:45:19 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 03:45:47 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLISION_H
# define COLLISION_H

# include "../types/common_types.h"

int     check_collision(t_position pos1, t_position pos2, int width, int height);
void    handle_entity_collision(int *x, int *y, int prev_x, int prev_y);
void    check_attack_collision(void);

#endif