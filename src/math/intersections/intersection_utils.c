/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:03:10 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/20 14:08:45 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

/*
prende una collisione e calcola tutte le cose utili per luci e riflessioni
*/
void	prepare_computations(t_intersection *intersection,
			t_ray *ray)
{
	ray_position(&intersection->point, ray, intersection->time);                    // Prende il raggio, calcola la posizione al tempo t dell’intersezione, e la salva come punto di impatto nello spazio 3D
	intersection->normal = normal_at(intersection->shape, &intersection->point);	// Chiama una funzione che restituisce la normale dell’oggetto nel punto colpito
	negate_vec(&intersection->eye, &ray->dir);										// L’eye vector è il verso opposto alla direzione del raggio, per illuminare dobbiamo sapere dall’oggetto verso la camera CONTRARIO
	intersection->eye.w = 0;														// 0 perché è un vettore, non un punto
	intersection->inside = false;													// Checj se l'intersezione è interna, se il raggio partiva da dentro 
	if (dot_product(&intersection->normal, &intersection->eye) < 0)
	{
		intersection->inside = true;
		negate_vec(&intersection->normal, &intersection->normal);
	}
	scale_vec(&intersection->over_point, &intersection->normal, EPSILON);			// Calcola over_point punto leggermente spostato fuori dall’oggetto, in direzione della normale per evitare problemi di precisione
	add_vec(&intersection->over_point, &intersection->point,
		&intersection->over_point);
	reflect_vector(&intersection->reflect_vec, &ray->dir,							// Usa la direzione del raggio e la normale per calcolare in che direzione rimbalzerebbe la luce
		&intersection->normal);
}


/*
prende la lista di intersezioni e restituisce l'intersezione più vicina con t > 0
ignora quelle dietro alla camera
*/
t_intersection	*hit(t_intersections *xs)
{
	double	min_time;
	int		i;
	int		idx;

	i = 0;
	idx = 0;
	min_time = INFINITY;											// significa “valore molto grande”
	while (i < xs->count)											// mi scorro tutte le intersezioni cercando quellq con il time minore , cioè piu vicina
	{
		if (xs->arr[i].time >= 0 && xs->arr[i].time < min_time)
		{
			min_time = xs->arr[i].time;
			idx = i;
		}
		i++;
	}
	if (min_time == INFINITY)
		return (NULL);												// Se nessuna intersezione è valida, ritorna NULL
	return (&xs->arr[idx]);											// Altrimenti, restituisce l’intersezione migliore
}
/*
Calcola la posizione lungo un raggio al tempo t.
Punto = Origine + t × Direzione 
P = O + t * D
*/
void	ray_position(t_vector *pos, const t_ray *ray, double time)
{
	pos->x = ray->dir.x * time + ray->origin.x;
	pos->y = ray->dir.y * time + ray->origin.y;
	pos->z = ray->dir.z * time + ray->origin.z;
	pos->w = 1;
}
/*
Serve per trasformare un raggio da mondo a object space, usando la matrice inversa della shape
Si moltiplicano sia il punto di partenza (origin) che la direzione (dir) per la matrice inversa della shape
*/
void	transform_ray(t_ray *transformed_ray, const t_ray *ray,
	const t_shape *shape)
{
	mat_vec_multiply(&transformed_ray->origin, &shape->inv_transf,
		&ray->origin);
	mat_vec_multiply(&transformed_ray->dir, &shape->inv_transf,
		&ray->dir);
}
