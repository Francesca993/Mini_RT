/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ambient.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 23:12:05 by francesca         #+#    #+#             */
/*   Updated: 2025/10/21 23:21:39 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* --- Ambiente --- */
/*
 * parse_ambient_line:
 *   Sintassi attesa (subject):
 *     A <ratio> <R,G,B>
 *   - ratio: double in [0..1]
 *   - R,G,B: interi in [0..255]
 *   Il puntatore 'rest_of_line' punta subito dopo l'ID 'A'.
 *   Ritorna 0 su successo, 1 su errore di formato o di range.
 *
typedef struct s_ambient
{
	double ratio;  // [0..1]
	t_color color; // normalizzato 0..1
	bool present;  // per validare che A compaia una sola volta
}	t_ambient;
*/

static inline int parse_ambient(const char **pcursor, t_color *color, double *ratio_value)
{
	const char *cursor;

	cursor = skip_spaces(*pcursor);
	if (!parse_double(&cursor, ratio_value)) /* 1) Leggi ratio */
		return (print_err_msg("Formato ratio di 'A' non valido"));
	if (*ratio_value < 0.0 || *ratio_value > 1.0)
		return (print_err_msg("Ratio di 'A' fuori range [0..1]"));
	cursor = skip_spaces(cursor); /* 1) Leggi ratio */
	if (*cursor == '\0')
		return (print_err_msg("Colore di 'A' mancante (atteso R,G,B)"));
	if (!parse_rgb(&cursor, color)) /* 1) Leggi RGB */
		return (print_err_msg("Formato colore di 'A' non valido (atteso R,G,B)"));
	cursor = skip_spaces(cursor); /* 4) Non devono esserci token extra dopo il colore */
	if (*cursor != '\0')
		return (print_err_msg("Token extra dopo il colore in 'A'"));
	*pcursor = cursor; /* commit avanzamento, MA SERVE?*/
	return (0);
}

int	parse_ambient_line(t_scene *scene, char *rest_of_line)
{
	const char  *cursor;
	t_color		color;
	double      ratio_value;
	
	if (scene == NULL || rest_of_line == NULL)
		return (print_err_msg("Parametro mancante per 'A'"));
	if (scene->ambient.present == true)
		return (print_err_msg("Ambiente 'A' definito più di una volta"));
	scene->n_ambient += 1;
	cursor = skip_spaces(rest_of_line);
	if (parse_ambient(&cursor, &color, &ratio_value) == 1)
		return (1);
	scene->ambient.intensity = ratio_value;
	scene->ambient.color = color;
	scene->ambient.present = true;
	return 0;
}



// int	parse_ambient_line(t_scene *scene, char *rest_of_line)
// {
// 	const char  *cursor;
// 	t_color		color;
// 	double      ratio_value;
	
// 	if (scene == NULL || rest_of_line == NULL)
// 		return (print_err_msg("Parametro mancante per 'A'"));
// 	if (scene->amb.present == true)
// 		return (print_err_msg("Ambiente 'A' definito più di una volta"));
// 	scene->n_ambient += 1;
// 	cursor = skip_spaces(rest_of_line);
// 	// if (!parse_double(&cursor, &ratio_value)) /* 1) Leggi ratio */
// 	// 	return (print_err_msg("Formato ratio di 'A' non valido"));
// 	// if (ratio_value < 0.0 || ratio_value > 1.0)
// 	// 	return (print_err_msg("Ratio di 'A' fuori range [0..1]"));
// 	// cursor = skip_spaces(cursor); /* 1) Leggi ratio */
// 	// if (*cursor == '\0')
// 	// 	return (print_err_msg("Colore di 'A' mancante (atteso R,G,B)"));
// 	// if (!parse_rgb(&cursor, &color)) /* 1) Leggi RGB */
// 	// 	return (print_err_msg("Formato colore di 'A' non valido (atteso R,G,B)"));
// 	// cursor = skip_spaces(cursor); /* 4) Non devono esserci token extra dopo il colore */
// 	// if (*cursor != '\0')
// 	// 	return (print_err_msg("Token extra dopo il colore in 'A'"));
// 	/* 5) Salva nella scena (normalizza il colore in [0..1]) */
// 	if (parse_ambient(&cursor, &color, &ratio_value) == 1)
// 		return (1);
// 	scene->amb.ratio = ratio_value;
// 	scene->amb.color = color;
// 	scene->amb.present = true;
// 	return 0;
// }
