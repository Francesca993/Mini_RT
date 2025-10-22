/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 16:25:15 by francesca         #+#    #+#             */
/*   Updated: 2025/10/21 23:02:04 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static inline int sphere(const char **pcursor, t_vector *point, double *diameter_value, t_color *color)
{
    const char *cursor;

	cursor = skip_spaces(*pcursor);
       
	if (!parse_vec3(&cursor, point, 1.0))  /* Posizione: x,y,z (Punto nello spazio: w = 1.0 ) */
		return (print_err_msg("Posizione Sphere non valida (atteso x,y,z senza spazi)"));
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Diametro mancante (atteso [0.0])"));
	if (!parse_double(&cursor, diameter_value)) /* Diametro */
		return (print_err_msg("Formato Diametro non valido"));
    if (*diameter_value <= 0.0)
    {
        return (print_err_msg("Valore Diametro negativo)"));
    }
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Colore di 'sp' mancante (atteso R,G,B)"));
	if (!parse_rgb(&cursor, color))/* Leggi R,G,B nel range [0..255] */
		return (print_err_msg("Formato colore di 'sp' non valido (atteso R,G,B)"));
	cursor = skip_spaces(cursor);/* Non devono esserci token extra dopo il colore */
	if (*cursor != '\0')
		return (print_err_msg("Token extra dopo il colore in 'sp'"));
    return (0);
}

/*
Sphere:
sp 0.0,0.0,20.6 12.6 10,0,255
∗ identifier: sp
∗ x, y, z coordinates of the sphere center: 0.0,0.0,20.6
∗ the sphere diameter: 12.6
∗ R,G,B colors in the range [0-255]: 10, 0, 255
*/
int parse_sphere(t_scene *scene, char *rest_of_line)
{
    const char  *cursor;
    t_vector    point;
    double      diameter_value;
	t_color		color;
    t_figures    payload;

    scene->n_spheres += 1;
        /* Controlli */
    if (scene == NULL || rest_of_line == NULL)
    {
        return (err_msg("Error\nSPHERE", scene->n_spheres, "Parametri mancanti"));
    }
	cursor = skip_spaces(rest_of_line);
    if (sphere(&cursor, &point, &diameter_value, &color) == 1)
        return (err_msg("SPHERE", scene->n_spheres, ""), 1);
    /* Sphere */
    payload.sphere.center = point;
    payload.sphere.radius = diameter_value;
    payload.sphere.color = color;
    /* 7) Append alla lista oggetti */
    if (object_list_append(scene, SPHERE, payload) != 0)
    {
        return (1); /* object_list_append ha già stampato l'errore */
    }
	return (0);
}


// int parse_sphere(t_scene *scene, char *rest_of_line)
// {
//     const char  *cursor;
//     t_vector    point;
//     double      diameter_value;
// 	t_color		color;
//     t_figures    payload;

//         /* Controlli */
//     if (scene == NULL || rest_of_line == NULL)
// 		return (print_err_msg("Parametro mancante per 'sp'"));
// 	cursor = skip_spaces(rest_of_line);
//     /* Posizione: x,y,z (senza spazi) */
// 	if (!parse_vec3(&cursor, &point, 1.0))
// 		return (print_err_msg("Posizione Sphere non valida (atteso x,y,z senza spazi)"));
// 	/* Punto nello spazio: w = 1.0 */
// 	// point.w = 1.0;
// 	/* almeno uno spazio prima del prossimo token */
// 	cursor = skip_spaces(cursor);
// 	if (*cursor == '\0')
// 		return (print_err_msg("Diametro mancante (atteso [0.0])"));
//     /* Leggi double per il Diametro */
// 	if (!parse_double(&cursor, &diameter_value))
// 		return (print_err_msg("Formato Diametro non valido"));
//     /* non può essere negativo no? */
//     if (diameter_value <= 0.0)
//         return (print_err_msg("Valore Diametro negativo)"));
// 	/* Atteso almeno uno spazio prima del colore */
// 	cursor = skip_spaces(cursor);
// 	if (*cursor == '\0')
// 		return (print_err_msg("Colore di 'sp' mancante (atteso R,G,B)"));
// 	/* Leggi R,G,B nel range [0..255] */
// 	if (!parse_rgb(&cursor, &color))
// 		return (print_err_msg("Formato colore di 'sp' non valido (atteso R,G,B)"));
// 	/* Non devono esserci token extra dopo il colore */
// 	cursor = skip_spaces(cursor);
// 	if (*cursor != '\0')
// 		return (print_err_msg("Token extra dopo il colore in 'sp'"));
//     /* Sphere */
//     payload.sphere.center = point;
//     payload.sphere.radius = diameter_value;
//     payload.sphere.color = color;
//     /* 7) Append alla lista oggetti */
//     if (object_list_append(scene, SPHERE, payload) != 0)
//         return 1; /* object_list_append ha già stampato l'errore */
// 	return (0);
// }
