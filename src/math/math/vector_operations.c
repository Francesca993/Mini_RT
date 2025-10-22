/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:40:56 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/14 18:22:08 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

// È la lunghezza della freccia -- serve per sapere quanto è “grande” un vettore
double	vec_magnitude(const t_vector *vec)
{
	return (sqrt(vec->x * vec->x + vec->y * vec->y \
			+ vec->z * vec->z));
}

/*
Prende una freccia e la rende lunga 1 (unitaria)
Serve per avere direzioni “pure”, indipendenti dalla distanza
Direzione rimane la stessa, ma la lunghezza è sempre 1
In raytracing si usa un sacco per i raggi e le normali
*/ 
void	normalize_vec(t_vector *vec)
{
	scale_vec(vec, vec, 1 / vec_magnitude(vec));
	vec->w = 0;
}

/*
 Misura quanto due frecce puntano nella stessa direzione Serve per:
 calcolare l’angolo tra due vettori,
 capire quanta luce colpisce una superficie (illuminazione diffusa)
 dice se due frecce guardano nella stessa direzione (positivo), opposta (negativo), o a 90° (zero)
 */
double	dot_product(const t_vector *v1, const t_vector *v2)
{
	return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z + v1->w * v2->w);
}
 /*
 Trova una freccia perpendicolare alle due date. Serve per:
 trovare la normale di una superficie ,
 costruire sistemi di coordinate locali
 risultato è in res
 */
void	cross_product(t_vector *res, const t_vector *v1, const t_vector *v2)
{
	res->x = v1->y * v2->z - v1->z * v2->y;
	res->y = v1->z * v2->x - v1->x * v2->z;
	res->z = v1->x * v2->y - v1->y * v2->x;
	res->w = 0;
}
/*
Calcola la distanza tra due punti. Serve per:
misurare quanto è lontano un punto dalla luce,
capire chi è più vicino alla camera. misura il percorso più corto tra due punti
*/
double	vec_distance(const t_vector *v1, const t_vector *v2)
{
	t_vector	sub;

	sub_vec(&sub, v1, v2);
	return (vec_magnitude(&sub));
}

/*
NOTA PER ME
(x,y,z) = dove sono / dove punta
w = 1 sei un punto nello spazio , puoi essere spostato
w = 0 sei una direzione/freccia , non puoi essere spostato, solo ruotato o scalato
*/
