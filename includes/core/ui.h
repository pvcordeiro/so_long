/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 03:10:20 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/19 04:03:38 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_H
# define UI_H

# include "../types/ui_types.h"

void	draw_health(void);
void	draw_ui_banners(void);
void	draw_sprint_icon(void);
void	draw_helper_message(void);
void	draw_collectible_counter(void);
void	draw_frame(void);
void	draw_mushroom(void);
void	draw_collectibles(void);
void	draw_exit_full(void);
void	draw_exit_part(bool draw_top);
void	init_ui_elements(void);
void	draw_end_game_screen(void);

#endif