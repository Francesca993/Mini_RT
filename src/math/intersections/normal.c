/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:04:37 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/20 16:38:10 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

/*
Questa funzione calcola la normale di un piano in un certo punto,Serve per calcolare riflessi, luci, ombre

*/
//t_vector	plane_normal(const t_shape *shape, const t_vector *itx_point)
t_vector	plane_normal(const t_shape *shape)
{
	t_vector	object_normal;														//la normale nel sistema di riferimento locale dell’oggetto
	t_vector	world_normal;														//la normale trasformata nello spazio mondo (world space)

	object_normal.x = 0;
	object_normal.y = 1;															// Per un piano orizzontale, la normale locale è sempre 1 verso l'alto
	object_normal.z = 0;
	object_normal.w = 0;															//indica che è un vettore, non un punto
	mat_vec_multiply(&world_normal, &shape->norm_transform, &object_normal);        // Altrimenti, trasformiamo la normale da object space a world space usando la matrice norm_transform
	world_normal.w = 0;																// Indichiamo che è un vettore
	normalize_vec(&world_normal);													// lo normalizziamo a 1 
	return (world_normal);															// lo restituiamo
}


t_vector	sphere_normal(t_vector *normal, const t_shape *shape,
				const t_vector *itx_point)
{
	t_vector	world_normal;

	mat_vec_multiply(normal, &shape->inv_transf, itx_point);						// Trasforma il punto di intersezione dal world space allo object space, perché la normale di una sfera si calcola facilmente in object space
	normalize_vec(normal);															// Calcola la direzione della normale
	normal->w = 0;																	// per indicare che è un vettore
	mat_vec_multiply(&world_normal, &shape->norm_transform, normal);				// Altrimenti, trasformiamo la normale in world space come fatto prima
	world_normal.w = 0;
	normalize_vec(&world_normal);
	return (world_normal);
}

/*
Di che oggetto si tratta?
Sfera → chiama sphere_normal()
Piano → plane_normal()
*/
t_vector	normal_at(const t_shape *shape, const t_vector *itx_point)
{
	t_vector	normal;
	t_vector	world_normal;

	if (shape->type == SPHERE)
		return (sphere_normal(&normal, shape, itx_point));
	else if (shape->type == PLANE)
		return (plane_normal(shape));
		//return (plane_normal(shape, itx_point));
	mat_vec_multiply(&world_normal, &shape->norm_transform, &normal);
	world_normal.w = 0;
	normalize_vec(&world_normal);
	return (world_normal);
}