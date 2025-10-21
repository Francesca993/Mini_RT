/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 13:28:00 by francesca         #+#    #+#             */
/*   Updated: 2025/10/10 16:00:32 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* Libera  gli objects della t_scena*/
void	scene_free(t_scene *scene)
{
    if (!scene)
        return;
    objlist_destroy(scene);  /* imposta anche obj_head/obj_end a NULL e contatori a 0 */
    scene->object_count = 0;
}
/* Mi serve per liberare l'array di OBJECTS_TYPES*/
void	free_array(char **arr)
{
	size_t	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

