/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:11:55 by francesca         #+#    #+#             */
/*   Updated: 2025/10/21 12:05:38 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static inline int plane(const char **pcursor, t_vector *point, t_vector *normal, t_color *color)
{
	const char *cursor;

	cursor = skip_spaces(*pcursor);
	
	if (!parse_vec3(&cursor, point, 1.0)) /* Posizione: x,y,z // point.w = 1.0; punto nello spazio: traslazioni applicate */
		return (print_err_msg("Posizione non valida (atteso x,y,z senza spazi)"));
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("3D normalized normal vector mancante (atteso nx,ny,nz)"));
	if (!parse_vec3(&cursor, normal, 0.0)) /* Normale: nx,ny,nz - vettore: traslazioni NON applicate */
		return (print_err_msg("3D normalized normal vector non valida (atteso nx,ny,nz senza spazi)"));
	if (!check_vec3direction(normal))/* Componenti in [-1,1] */
		return (print_err_msg("3D normalized normal vectorfuori range [-1,1]"));
    /* Controllo vettore nullo e normalizzazione  	//VIENE FATTO DOPO NELLA PARTE MATH */
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Colore mancante"));
	if (!parse_rgb(&cursor, color))/* Leggi R,G,B nel range [0..255] */
		return (print_err_msg("Formato colore non valido (atteso R,G,B)"));
	cursor = skip_spaces(cursor);
	if (*cursor != '\0')
		return (print_err_msg ("Token extra dopo il colore"));
	return (0);
}

/*
∗ identifier: pl
∗ x, y, z coordinates of a point in the plane: 0.0,0.0,-10.0
∗ 3D normalized normal vector, in the range [-1,1] for each x, y, z axis:
0.0,1.0,0.0
∗ R,G,B colors in the range [0-255]: 0,0,225
*/
int parse_plane(t_scene *scene, char *rest_of_line)
{
    const char	*cursor;
	t_vector	point;
	t_vector	normal;
    t_figures   payload;
    t_color		color;

	scene->n_planes += 1;
	
	if (scene == NULL || rest_of_line == NULL)
		return (err_msg("Error\nPLANE", scene->n_planes, "Parametri mancanti"));
	cursor = skip_spaces(rest_of_line);
	if (plane(&cursor, &point, &normal, &color) == 1)
		return (err_msg("PLANE", scene->n_planes, ""), 1);
    /* Plane */
    payload.plane.point = point;
    payload.plane.normal = normal;
    payload.plane.color = color;
    /* 7) Append alla lista oggetti */
    if (object_list_append(scene, PLANE, payload) != 0)
        return 1; /* object_list_append ha già stampato l'errore */
	return (0);
}



// int parse_plane(t_scene *scene, char *rest_of_line)
// {
//     const char	*cursor;
// 	t_vector	point;
// 	t_vector	normal;
//     t_figures   payload;
//     t_color		color;

// 	if (scene == NULL || rest_of_line == NULL)
// 		return (err_msg("PLANE", scene->n_planes, "Parametri mancanti"));
// 	cursor = skip_spaces(rest_of_line);
// 	/* 1) Posizione: x,y,z */
// 	if (!parse_vec3(&cursor, &point, 1.0))
// 		return (err_msg("PLANE", scene->n_planes, "Posizione non valida (atteso x,y,z senza spazi)"));
// 	// point.w = 1.0; /* punto nello spazio: traslazioni applicate */
// 	cursor = skip_spaces(cursor);
// 	if (*cursor == '\0')
// 		return (err_msg("PLANE", scene->n_planes, "3D normalized normal vector mancante (atteso nx,ny,nz)"));
// 	/* Normale: nx,ny,nz */
// 	if (!parse_vec3(&cursor, &normal, 0.0))
// 		return (err_msg("PLANE", scene->n_planes, "3D normalized normal vector non valida (atteso nx,ny,nz senza spazi)"));
// 	// normal.w = 0.0; /* vettore: traslazioni NON applicate */
// 	/* Componenti in [-1,1] */
// 	if (!check_vec3direction(&normal))
// 		return (err_msg("PLANE", scene->n_planes, "3D normalized normal vectorfuori range [-1,1]"));
//     /* Controllo vettore nullo e normalizzazione  	//VIENE FATTO DOPO NELLA PARTE MATH */
// 	cursor = skip_spaces(cursor);
// 	if (*cursor == '\0')
// 		return (err_msg("PLANE", scene->n_planes, "Colore mancante"));
//     /* Leggi R,G,B nel range [0..255] */
// 	if (!parse_rgb(&cursor, &color))
// 		return (err_msg("PLANE", scene->n_planes, "Formato colore non valido (atteso R,G,B)"));
// 	/* Non devono esserci token extra dopo il colore */
// 	cursor = skip_spaces(cursor);
// 	if (*cursor != '\0')
// 		return (err_msg("PLANE", scene->n_planes, "Token extra dopo il colore"));
//     /* Sphere */
//     payload.plane.point = point;
//     payload.plane.normal = normal;
//     payload.plane.color = color;
//     /* 7) Append alla lista oggetti */
//     if (object_list_append(scene, OBJ_PLANE, payload) != 0)
//         return 1; /* object_list_append ha già stampato l'errore */
// 	return (0);
// }
