/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:31:04 by francesca         #+#    #+#             */
/*   Updated: 2025/10/21 12:05:13 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#import "miniRT.h"

/*
 Cylinder:
cy 50.0,0.0,20.6 0.0,0.0,1.0 14.2 21.42 10,0,255
∗ identifier: cy
∗ x, y, z coordinates of the center of the cylinder: 50.0,0.0,20.6
∗ 3D normalized vector of axis of cylinder, in the range [-1,1] for each x, y,
z axis: 0.0,0.0,1.0
∗ the cylinder diameter: 14.2
∗ the cylinder height: 21.42
∗ R, G, B colors in the range [0,255]: 10, 0, 255

typedef struct s_cylinder
{
	t_vector base;   // punto base lungo l'asse (convenz.
	t_vector axis;   // direzione normalizzata
	double radius; // diametro
	double height; // altezza finita
	t_color color; // 0..1
}			t_cylinder;
*/

static inline int cylinder(const char **pcursor, t_vector *axis, t_vector *base, double *radius)
{
    const char *cursor;

	cursor = skip_spaces(*pcursor);
	
	if (!parse_vec3(&cursor, base, 1.0)) /* Posizione: x,y,z punto nello spazio: traslazioni applicate*/
		return (print_err_msg("Posizione cilidro non valida (atteso x,y,z)"));
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("3D normal vector cylinder mancante (atteso nx,ny,nz)"));
	if (!parse_vec3(&cursor, axis, 0.0))/* Normale: nx,ny,nz */
		return (print_err_msg("3D normal vector of axis of cylinder non valida (atteso nx,ny,nz senza spazi)"));
	// axis.w = 0.0; /* vettore: traslazioni NON applicate */ //LO IMPOSTO QUI O NEL MATH?
	if (!check_vec3direction(axis))// /* 2.a) Componenti in [-1,1] */
		return (1);
		// return (print_err_msg("3D normal vector of axis of cylinder fuori range [-1,1]"));
    /* Controllo vettore nullo e normalizzazione */ // VIENE FATTO DOPO NEL MATH 
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("diametro del cilindro mancante"));
	if (!parse_double(&cursor, radius)) /* Leggi diametro */
		return (print_err_msg("Formato diametro di 'cy' non valido"));
	if (*radius <= 0.0)
		return print_err_msg("Il diametro del cilindro deve essere > 0");
	cursor = skip_spaces(cursor);

	*pcursor = cursor;

	return (0);
	
}

static inline int cylinder_2(const char **pcursor, double *height, t_color *color)
{
	const char *cursor;

	cursor = skip_spaces(*pcursor);
	if (*cursor == '\0')
		return (print_err_msg("altezza del cilindro mancante"));
	if (!parse_double(&cursor, height)) /* Leggi altezza */
		return (print_err_msg("Formato altezza di 'cy' non valido"));
	if (*height <= 0.0)
		return print_err_msg("L'altezza del cilindro deve essere > 0");
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("colore del cilindro mancante"));
	if (!parse_rgb(&cursor, color)) /* Leggi R,G,B nel range [0..255] */
		return (print_err_msg("Formato colore di 'cy' non valido (atteso R,G,B)"));
	cursor = skip_spaces(cursor); /* Non devono esserci token extra dopo il colore */
	if (*cursor != '\0')
		return (print_err_msg("Token extra dopo il colore in 'cy'"));
	return (0);
}

int parse_cylinder(t_scene *scene, char *rest_of_line)
{
	const char *cursor;
	t_vector base;
	t_vector axis; // direzione normalizzata
	double radius; 
	double height;
	t_color color;
	t_figures   payload;

	scene->n_cylinders += 1;
	if (scene == NULL || rest_of_line == NULL)
		return (err_msg("Error\nCYLINDER", scene->n_cylinders, "Parametri mancanti"));
	cursor = skip_spaces(rest_of_line);
	if (cylinder(&cursor, &axis, &base, &radius) == 1)
		return(err_msg("CYLINDER", scene->n_cylinders, ""));
	if (cylinder_2(&cursor, &height, &color) == 1)
		return(err_msg("CYLINDER", scene->n_cylinders, ""));
	
    /* Cylinder*/
	payload.cylinder.base = base;
	payload.cylinder.axis = axis;
	payload.cylinder.color = color;
	payload.cylinder.radius = radius;
	payload.cylinder.height = height;
    /* 7) Append alla lista oggetti */
    if (object_list_append(scene, CYLINDER, payload) != 0)
        return 1; /* object_list_append ha già stampato l'errore */
	return (0);
}



// int parse_cylinder(t_scene *scene, char *rest_of_line)
// {
// 	const char *cursor;
// 	t_vector base;
// 	t_vector axis; // direzione normalizzata
// 	double radius; 
// 	double height;
// 	t_color color;
// 	t_figures   payload;

// 	if (scene == NULL || rest_of_line == NULL)
// 		return (print_err_msg("Parametri mancanti per 'cy'"));
// 	cursor = skip_spaces(rest_of_line);
// 	/* Posizione: x,y,z */
// 	if (!parse_vec3(&cursor, &base, 1.0))
// 		return (print_err_msg("Posizione cilidro non valida (atteso x,y,z)"));
// 	// base.w = 1.0; /* punto nello spazio: traslazioni applicate */
// 	cursor = skip_spaces(cursor); // Salto gli spazi
// 	if (*cursor == '\0')
// 		return (print_err_msg("3D normal vector cylinder mancante (atteso nx,ny,nz)"));
// 	if (!parse_vec3(&cursor, &axis, 0.0))/* Normale: nx,ny,nz */
// 		return (print_err_msg("3D normal vector of axis of cylinder non valida (atteso nx,ny,nz senza spazi)"));
// 	// axis.w = 0.0; /* vettore: traslazioni NON applicate */ //LO IMPOSTO QUI O NEL MATH?
// 	if (!check_vec3direction(&axis))// /* 2.a) Componenti in [-1,1] */
// 		return (print_err_msg("3D normal vector of axis of cylinder fuori range [-1,1]"));
//     /* Controllo vettore nullo e normalizzazione */ // VIENE FATTO DOPO NEL MATH 
// 	cursor = skip_spaces(cursor);
// 	if (*cursor == '\0')
// 		return (print_err_msg("diametro del cilindro mancante"));
// 	/* Leggi diametro */
// 	if (!parse_double(&cursor, &radius))
// 		return (print_err_msg("Formato diametro di 'cy' non valido"));
// 	if (radius <= 0.0)
// 		return print_err_msg("Il diametro del cilindro deve essere > 0");
// 	cursor = skip_spaces(cursor);
// 	if (*cursor == '\0')
// 	return (print_err_msg("altezza del cilindro mancante"));
// 	/* Leggi altezza */
// 	if (!parse_double(&cursor, &height))
// 		return (print_err_msg("Formato altezza di 'cy' non valido"));
// 	if (height <= 0.0)
// 		return print_err_msg("L'altezza del cilindro deve essere > 0");
// 	cursor = skip_spaces(cursor);
// 	if (*cursor == '\0')
// 		return (print_err_msg("colore del cilindro mancante"));
// 	/* Leggi R,G,B nel range [0..255] */
// 	if (!parse_rgb(&cursor, &color))
// 		return (print_err_msg("Formato colore di 'cy' non valido (atteso R,G,B)"));
// 	/* Non devono esserci token extra dopo il colore */
// 	cursor = skip_spaces(cursor);
// 	if (*cursor != '\0')
// 		return (print_err_msg("Token extra dopo il colore in 'cy'"));
//     /* Cylinder*/
// 	payload.cylinder.base = base;
// 	payload.cylinder.axis = axis;
// 	payload.cylinder.color = color;
// 	payload.cylinder.radius = radius;
// 	payload.cylinder.height = height;
//     /* 7) Append alla lista oggetti */
//     if (object_list_append(scene, OBJ_CYLINDER, payload) != 0)
//         return 1; /* object_list_append ha già stampato l'errore */
// 	return (0);
// }


