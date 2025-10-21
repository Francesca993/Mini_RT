/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:29:04 by francesca         #+#    #+#             */
/*   Updated: 2025/10/20 17:02:19 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <string.h>

//Controlla che il file sia .rt
//Check if the file has the extenctrion .rt
int has_rt_extension(const char *path)
{
	size_t len;

	if (path == NULL)
		return (0);
	len = ft_strlen(path);
	if (len < 3)
		return (0);
	if (ft_strcmp(path + len - 3, ".rt") == 0)
		return (1);
	return (0);
}

/* Ritorna 1 se id è uno dei token ammessi nel mandatory */
int is_valid_identifier(const char *id)
{
    if (!id || !*id) return 0;
    if (ft_strcmp(id, "A") == 0)  return 1;
    if (ft_strcmp(id, "C") == 0)  return 1;
    if (ft_strcmp(id, "L") == 0)  return 1;
    if (ft_strcmp(id, "sp") == 0) return 1;
    if (ft_strcmp(id, "pl") == 0) return 1;
    if (ft_strcmp(id, "cy") == 0) return 1;
    return 0;
}

/* Controlla che i Valori delle coordinate della camra siano nei range ammessi
dal subject:
3D normalized orientation vector, 
in the range [-1,1] 
for each x, y, z axis: 0.0,0.0,1.0 */
int check_vec3direction(t_vector *direction_value)
{
    /* 2.a) Componenti in [-1,1] */
	if (direction_value->x < -1.0 || direction_value->x > 1.0
		|| direction_value->y < -1.0 || direction_value->y > 1.0
		|| direction_value->z < -1.0 || direction_value->z > 1.0)
		{
			print_err_msg("3D normal vector of axis of cylinder fuori range [-1,1]");
			return (0);
		}
	if (direction_value->x == 0 && direction_value->y == 0 && direction_value->z == 0)
	{
		print_err_msg("Vettore di normalizzazione nullo");
		return (0);
	}
    return (1);
}
/* Check che ci siano Presenze obbligatorie e Unicita di A, C, L una sola volta. 
mi serve qualcosa che a fine ciclo mi controlli che ci siano tutti e 3 e almeno uno tra gli oggetti
per il momento stamperà solo un messaggio di errore sennò non andiamo piu avanti*/
/*
typedef struct s_scene
{
	// istanze reali (da riempire via parser)
	t_ambient amb; // A (una sola nel mandatory)
	t_camera cam;  // C (una sola nel mandatory)
	t_light light; // L (una sola nel mandatory)
	// contatori utili durante il parsing/validazione
	int n_ambient; // per rilevare doppie dichiarazioni di A
	int n_camera;  // per C
	int n_lights;  // per L (bonus: servirà un array)
	int		n_spheres;
	int		n_planes;
	int		n_cylinders;
}			t_scene;
*/

/* Fa il check su presenze e unicità */
int check_presence(t_scene *scene)
{
	if (scene == NULL)
		return (print_err_msg("Error: scena non valida (NULL)"));

	/* Presenza minima: A, C, L */
	if (scene->amb.present == false || scene->n_ambient == 0)
		return (print_err_msg("Error: Ambiente 'A' mancante"));
	if (scene->cam.present == false || scene->n_camera == 0)
		return (print_err_msg("Error: Camera 'C' mancante"));
	if (scene->light.present == false || scene->n_lights == 0)
		return (print_err_msg("Error: Luce 'L' mancante"));

	/* Duplicati proibiti (subject: maiuscole una sola volta) */
	if (scene->n_ambient > 1)
		return (print_err_msg("Error: Ambiente 'A' definito piu' di una volta"));
	if (scene->n_camera > 1)
		return (print_err_msg("Error: Camera 'C' definita piu' di una volta"));
	if (scene->n_lights > 1)
		return (print_err_msg("Error: Luce 'L' definita piu' di una volta"));

	/* Presenza di almeno un oggetto */
	if (scene->n_planes + scene->n_spheres + scene->n_cylinders == 0)
		return(print_err_msg("Error: Deve esserci almeno un cy o pl o sp"));
	return (0);
}
