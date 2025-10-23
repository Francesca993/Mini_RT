/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 10:37:23 by francesca         #+#    #+#             */
/*   Updated: 2025/10/22 20:47:01 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
if	(parse_light_line(scene, rest_out) != 0)
// Light (L pos, intensità [0..1], colore)
typedef struct s_light
{
	t_vector		pos;
	double intensity; // [0..1]
	t_color color;    // normalizzato 0..1
	bool			present;
}					t_light;
L -40.0,50.0,0.0    0.6    10,0,255
∗ identifier: L
∗ x, y, z coordinates of the light point: -40.0,50.0,0.0
∗ the light brightness ratio in the range [0.0,1.0]: 0.6
∗ (unused in mandatory part) R, G, B colors in the range [0-255]: 10, 0,
255
*/

// int	parse_light_line(t_scene *scene, char *rest_of_line)
// {
//     const char  *cursor;
//     t_vector    position_value;
//     double      light_ratiovalue;
// 	t_color		color;

//     /* Controlli */
//     if (scene == NULL || rest_of_line == NULL)
// 		return (print_err_msg("Parametro mancante per 'L'"));
//     if (scene->light.present == true)
// 		return (print_err_msg("Luce 'L' definito più di una volta"));
// 	cursor = skip_spaces(rest_of_line);
//     /* Posizione: x,y,z */
// 	if (!parse_vec3(&cursor, &position_value, 1.0))
		/* la posizione è → w=1.0  perchè si muove */
// 		return (print_err_msg("Posizione Luce non valida (atteso x,y,z senza spazi)"));
// 	cursor = skip_spaces(cursor); 	/* almeno uno spazio prima del prossimo token */
// 	if (*cursor == '\0')
// 		return (print_err_msg("Luminosità luce <ratio> mancante (atteso [0.0,1.0])"));
// 	if (!parse_double(&cursor, &light_ratiovalue)) /* Leggi ratio */
// 		return (print_err_msg("Formato ratio di 'L' non valido"));
// 	if (light_ratiovalue < 0.0 || light_ratiovalue > 1.0)
		/* check range ratio */
// 		return (print_err_msg("Ratio di 'L' fuori range [0..1]"));
// 	cursor = skip_spaces(cursor);
// 	if (*cursor == '\0')
// 		return (print_err_msg("Colore di 'L' mancante (atteso R,G,B)"));
// 	if (!parse_rgb(&cursor, &color)) /* Leggi R,G,B nel range [0..255] */
// 		return (print_err_msg("Formato colore di 'L' non valido (atteso R,G,B)"));
// 	cursor = skip_spaces(cursor);
// 	if (*cursor != '\0')/* Non devono esserci token extra dopo il colore */
// 		return (print_err_msg("Token extra dopo il colore in 'L'"));
// 	/* Salva nella scena (normalizza il colore in [0..1]) */
//     scene->light.pos = position_value;
//     scene->light.intensity = light_ratiovalue;
//     scene->light.color = color;
//     scene->light.present   = true;
//     scene->n_lights += 1;
// 	return (0);
// }

/* Cuore della funzione che legge effettivamente la riga  */
static inline int	parse_light(const char **pcursor, t_vector *position_out,
		double *ratio_out, t_color *color_out)
{
	const char	*cursor;

	if (!pcursor || !*pcursor || !position_out || !ratio_out || !color_out)
		return (print_err_msg("Parametri mancanti per 'L'"));
	cursor = skip_spaces(*pcursor);
	if (!parse_vec3(&cursor, position_out, 1.0))
		/* Posizione: x,y,z (w=1.0 perché è un punto) */
		return (print_err_msg("Posizione Luce non valida (atteso x,y,z senza spazi)"));
	cursor = skip_spaces(cursor); /* salta gli spazi*/
	if (*cursor == '\0')
		return (print_err_msg("Luminosità luce <ratio> mancante (atteso [0.0,1.0])"));
	if (!parse_double(&cursor, ratio_out)) /* Ratio */
		return (print_err_msg("Formato ratio di 'L' non valido"));
	if (*ratio_out < 0.0 || *ratio_out > 1.0) /* Controllo range ratio*/
		return (print_err_msg("Ratio di 'L' fuori range [0..1]"));
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Colore di 'L' mancante (atteso R,G,B)"));
	if (!parse_rgb(&cursor, color_out)) /* Colore RGB */
		return (print_err_msg("Formato colore di 'L' non valido (atteso R,G,B)"));
	cursor = skip_spaces(cursor);
	if (*cursor != '\0') /* non deve esserci nulla dopo */
		return (print_err_msg("Token extra dopo il colore in 'L'"));
	*pcursor = cursor; /* commit avanzamento */
	return (0);
}

int	parse_light_line(t_scene *scene, char *rest_of_line)
{
	const char	*cursor;
	t_vector	position_value;
	double		light_ratiovalue;
	t_color		color;

	/* Controlli */
	if (scene == NULL || rest_of_line == NULL)
		return (print_err_msg("Parametro mancante per 'L'"));
	if (scene->n_lights >= 1 || scene->lights[0].present == true)
		return (print_err_msg("Luce 'L' definito più di una volta"));
	cursor = skip_spaces(rest_of_line);
	/* Posizione: x,y,z */
	if (parse_light(&cursor, &position_value, &light_ratiovalue, &color) == 1)
		return (1);
	/* Salva nella scena (normalizza il colore in [0..1]) */
	scene->lights[0].position = position_value;
	scene->lights[0].intensity = light_ratiovalue;
	scene->lights[0].color = color;
	scene->lights[0].present = true;
	scene->n_lights += 1;
	return (0);
}
