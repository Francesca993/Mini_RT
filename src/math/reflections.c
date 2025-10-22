/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflections.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 11:40:00 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/20 16:40:06 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

t_color	reflection_color(t_intersection *itx, t_scene *scene, int light_idx)
{
	t_color			final_color;
	t_color			light_color;
	t_color			reflected;

	ft_bzero(&final_color, sizeof(t_color));
	light_color = phong(itx, scene, light_idx);
	add_colors(&final_color, &final_color, &light_color);
	add_colors(&final_color, &final_color, &reflected);
	return (final_color);
}


