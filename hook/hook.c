/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <fmontini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:29:08 by fmontini          #+#    #+#             */
/*   Updated: 2025/09/29 11:48:44 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	close_hook(void *param)
{
	t_app *a = (t_app *)param;
	if (a->win)
		mlx_destroy_window(a->mlx, a->win);
	exit(0);
	return (0);
}

int	key_hook(int keycode, void *param)
{
	t_app *a = (t_app *)param;
	if (keycode == KEY_ESC)
	{
		if (a->win)
			mlx_destroy_window(a->mlx, a->win);
		exit(0);
	}
	return (0);
}