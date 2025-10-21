/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_camera.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 11:12:18 by francesca         #+#    #+#             */
/*   Updated: 2025/10/20 17:06:16 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
C -50.0,0,20 0,0,1 70
∗ identifier: C
∗ x, y, z coordinates of the viewpoint: -50.0,0,20
∗ 3D normalized orientation vector, in the range [-1,1] for each x, y, z axis:
0.0,0.0,1.0
∗ FOV: Horizontal field of view in degrees in the range [0,180]: 70
*/


/* controllo normalizzazione SENZA sqrt: |x^2+y^2+z^2 - 1| <= eps2
è per controllare che il vettore direzione della camera sia davvero normalizzato,
cioè che la sua lunghezza (modulo) sia uguale a 1 */
// static int	is_normalized_len2(double x, double y, double z, double eps2)
// {
// 	double	len2;
// 	double	diff;

// 	len2 = x * x + y * y + z * z;
// 	diff = len2 - 1.0;
// 	if (diff < 0.0)
// 		diff = -diff;
// 	return (diff <= eps2);
// }

/* ============================ parse_camera_line ============================ */
/* Check se ambiente fosse già presente, parse dei primi vettori,
e check che riesca,
la posizione si sposta quindi imposto il valore w. Vado avanti assicurandomi che 
ci sia almeno uno spazio,converto la direzione,
che non avrà w. Fa il check che i valori siano nei range del subbject.
Controlla che il vettore sia normalizzato, fa di nuovo il check su gli spazi,
converte il FOV, ne controlla il range che sia nei
limiti del subject,e poi lo converte in radiani. controlla che sia fine stringa 
o solo spazi dopo, e salva tutto nella t_scene.
*/
// int	parse_camera_line(t_scene *scene, char *rest_of_line)
// {
// 	const char	*cursor;
// 	t_vector	position_value;
// 	t_vector	direction_value;
// 	int			fov_degrees_integer;

// 	if (scene == NULL || rest_of_line == NULL)
// 		return (print_err_msg("Parametri mancanti per 'C'"));
// 	if (scene->cam.present == true)
// 		return (print_err_msg("Camera 'C' definita più di una volta"));
// 	cursor = skip_spaces(rest_of_line);
// 	if (!parse_vec3(&cursor, &position_value, 1.0)) /* Posizione: x,y,z, w → la posizione è un punto → w=1.0  perchè si sposta */
// 		return (print_err_msg("Posizione camera non valida (atteso x,y,z senza spazi)"));
// 	cursor = skip_spaces(cursor);
// 	if (*cursor == '\0')
// 		return (print_err_msg("Direzione camera mancante (atteso nx,ny,nz)"));
// 	if (!parse_vec3(&cursor, &direction_value, 0.0)) /* Direzione: nx,ny,nz, nw  → direzione è un vettore → w=0.0 perchè NON si sposta*/
// 		return (print_err_msg("Direzione camera non valida (atteso nx,ny,nz senza spazi)"));
// 	if (!check_vec3direction(&direction_value)) /* Componenti in [-1,1] */
// 		return (print_err_msg("Componenti direzione fuori range [-1,1]"));
// 	/*  QUI SI PUO INSERIRE CALCOLO DI NORMALIZZAZIONE FATTO DALLA PARTE MATH */
// 	cursor = skip_spaces(cursor);
// 	if (*cursor == '\0')
// 		return (print_err_msg("FOV della camera mancante"));
// 	if (!parse_int(&cursor, &fov_degrees_integer)) /* FOV in gradi [0..180]  */
// 		return (print_err_msg("FOV della camera non valido (atteso intero)"));
// 	if (fov_degrees_integer < 0 || fov_degrees_integer > 180)
// 		return (print_err_msg("FOV della camera fuori range [0..180]"));
// 	cursor = skip_spaces(cursor); /* Niente token extra */
// 	if (*cursor != '\0')
// 		return (print_err_msg("Token extra dopo il FOV della camera"));
// 	/* Salvataggio in scena */
// 	scene->cam.pos = position_value;
// 	scene->cam.dir = direction_value;
// 	scene->cam.fov_deg = fov_degrees_integer;
// 	scene->cam.present = true;
// 	scene->n_camera += 1;
// 	return (0);
// }
/*
🌍 Contesto: coordinate omogenee
In grafica 3D, un vettore 3D (x, y, z) viene a volte esteso a 4 componenti
 (x, y, z, w) per poter usare le matrici 4×4 (trasformazioni affini, proiezioni, ecc.).
Questo si chiama sistema di coordinate omogenee.
📐 Differenza tra punto e direzione
La camera ha:
una posizione → dove si trova nel mondo (es: sul tavolo, a 1 metro dal pavimento),
una direzione → dove guarda (es: verso la porta).
Tipo	Coordinate	Significato di w
Punto (posizione nello spazio)	(x, y, z, 1)	w = 1.0 indica che si sposta con le traslazioni
Vettore (direzione o velocità)	(x, y, z, 0)	w = 0.0 indica che non è influenzato dalle traslazioni
Quando moltiplichi un vettore o un punto per una matrice di trasformazione 4×4, w determina se 
la traslazione della matrice deve applicarsi o no.
Mettere w = 1.0 per i punti e w = 0.0 per i vettori:
Campo	Tipo	w	Effetto
scene->cam.pos	posizione nello spazio	1.0	si sposta con la camera
scene->cam.dir	direzione di vista	0.0	non si sposta, solo ruota
light.pos	punto nello spazio	1.0	si sposta
normal	vettore direzione	0.0	non si sposta
*/
static inline int parse_camera(const char **pcursor, t_vector *position_value, t_vector *direction_value, int *fov_degrees_integer)
{
	const char *cursor;
	
	cursor = skip_spaces(*pcursor);
	if (!parse_vec3(&cursor, position_value, 1.0)) /* Posizione: x,y,z, w → la posizione è un punto → w=1.0  perchè si sposta */
		return (print_err_msg("Posizione camera non valida (atteso x,y,z senza spazi)"));
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("Direzione camera mancante (atteso nx,ny,nz)"));
	if (!parse_vec3(&cursor, direction_value, 0.0)) /* Direzione: nx,ny,nz, nw  → direzione è un vettore → w=0.0 perchè NON si sposta*/
		return (print_err_msg("Direzione camera non valida (atteso nx,ny,nz senza spazi)"));
	if (!check_vec3direction(direction_value)) /* Componenti in [-1,1] */
		return (err_msg("Camera", 1, ""));
	/*  QUI SI PUO INSERIRE CALCOLO DI NORMALIZZAZIONE FATTO DALLA PARTE MATH */
	cursor = skip_spaces(cursor);
	if (*cursor == '\0')
		return (print_err_msg("FOV della camera mancante"));
	if (!parse_int(&cursor, fov_degrees_integer)) /* FOV in gradi [0..180]  */
		return (print_err_msg("FOV della camera non valido (atteso intero)"));
	if (*fov_degrees_integer < 0 || *fov_degrees_integer > 180)
		return (print_err_msg("FOV della camera fuori range [0..180]"));
	cursor = skip_spaces(cursor); /* Niente token extra */
	if (*cursor != '\0')
		return (print_err_msg("Token extra dopo il FOV della camera"));
	return (0);
}

int	parse_camera_line(t_scene *scene, char *rest_of_line)
{
	const char	*cursor;
	t_vector	position_value;
	t_vector	direction_value;
	int			fov_degrees_integer;

	if (scene == NULL || rest_of_line == NULL)
		return (print_err_msg("Parametri mancanti per 'C'"));
	if (scene->cam.present == true)
		return (print_err_msg("Camera 'C' definita più di una volta"));
	cursor = skip_spaces(rest_of_line);
	if (parse_camera(&cursor, &position_value, &direction_value, &fov_degrees_integer) == 1)
		return (1);
	/* Salvataggio in scena */
	scene->cam.pos = position_value;
	scene->cam.dir = direction_value;
	scene->cam.fov_deg = fov_degrees_integer;
	scene->cam.present = true;
	scene->n_camera += 1;
	return (0);
}
