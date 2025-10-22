/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 16:39:21 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/17 10:41:31 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

/*
In base all’intersezione (il punto dove il raggio ha colpito),
dice che colore ha quell’oggetto lì
Usa semplicemente il colore base dello shape
*/
t_color	get_shape_color(t_intersection *itx)
{
	return (itx->shape->props.color);
}

/*
calcola quanta luce colpisce un punto su un oggetto sia diffusa che riflessa
IMPLEMENTA POI LA SPOT LIGHT
*/
bool	get_specular_and_diffuse(t_scene *scene, int light_idx,
	t_intersection *itx, t_phong *phong)
{
	double		light_dot_normal;                                                       // quanto la luce punta nella stessa direzione della superficie
	t_vector	light_v;                                                                // il vettore che va dal punto colpito alla luce
	t_vector	reflect_v;                                                              // vettore riflesso della luce (serve per calcolare il brillio)

	sub_vec(&light_v, &scene->lights[light_idx].position, &itx->over_point);            // capiamo da dove arriva la luce rispetto a quel punto.
	normalize_vec(&light_v);                                                            // normalizziamo per facilitare calcoli
	itx->normal.w = 0;                                                                  // la normale non ha componente w 
	light_dot_normal = dot_product(&light_v, &itx->normal);                             // Calcoliamo quanto la luce sta puntando nella direzione della superficie, > 0 → la luce illumina il punto, 0 → la luce è di lato, < 0  → la luce arriva da dietro
	if (light_dot_normal < 0 || is_shadowed(scene, light_idx, &itx->over_point))        // se la luce arriva da dietro, o c’è un altro oggetto davanti
		return (false);                                                                 // punto non illuminato
	mult_color(&phong->diffuse, &phong->effective_color, light_dot_normal               // Calcoliamo la luce diffusa, cioè quella che si spalma sul punto
		* itx->shape->props.diffuse * scene->lights[light_idx].intensity);
		mult_color(&phong->diffuse, &phong->diffuse, 0.8);                              //la luce diffusa un po’ meno forte 80%
	negate_vec(&light_v, &light_v);                                                     // Invertiamo la direzione della luce er calcolare come si riflette la luce verso l’occhio
	reflect_vector(&reflect_v, &light_v, &itx->normal);                                 // Calcoliamo la direzione del riflesso della luce, come se fosse uno specchio
	calculate_specular(&reflect_v, itx, phong, &scene->lights[light_idx]);              // Calcoliamo quanto è brillante il riflesso su questo punto
	return (true);                                                                      // punto illuminato
}

// NOTE PER ME 
/*
FORMULA LUCE DIFFUSA 
colore finale =
    colore dell’oggetto *
    quanta luce arriva (dot) *
    quanto l’oggetto è 'diffuso' (tipo stoffa) *
    intensità della luce
*/