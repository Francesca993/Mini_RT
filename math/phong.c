/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phong.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:46:03 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/21 12:07:28 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

/*
Calcola la luce ambientale che colpisce un punto
*/

t_color	get_ambient(t_scene *scene, t_color patter_color)
{
	t_color	ambient;

	ambient.a = 0; // trasparenza per dopo
	mult_color(&ambient, &patter_color,                             //Moltiplica il colore dell'oggetto (patter_color) per quanto è forte la luce ambientale (intensity)
		scene->ambient.intensity);
	blend_colors(&ambient, &ambient, &scene->ambient.color);        //Mescola il colore ambientale ottenuto con il colore reale della luce ambientale
	return (ambient);
}

/*
Calcola il riflesso (quella parte brillante che si vede solo da certe angolazioni)
*/
void	calculate_specular(t_vector *reflect_v, t_intersection *itx,
			t_phong *phong, t_light *light)
{
	double	reflect_dot_eye;

	reflect_dot_eye = dot_product(reflect_v, &itx->eye);                // Calcola quanto il riflesso va nella direzione degli occhi dell’osservatore.
	if (reflect_dot_eye <= 0)                                          // se il riflesso non è visibile → metti a 0 la luce riflessa
		ft_bzero(&phong->specular, sizeof(t_color));
	else
		mult_color(&phong->specular, &light->color,                    // specular = colore della luce
            // Moltiplicato per quanto l'oggetto riflette specular
            // Moltiplicato per quanto è lucido shininess - pow per far sembrare i riflessi più piccoli e realistici
            // Moltiplicato per l’intensità della luce
			itx->shape->props.specular * 
			pow(reflect_dot_eye, itx->shape->props.shininess)          
			* light->intensity);                                       
}
/*
Controlla se un oggetto sta facendo ombra su questo punto
*/
bool	is_shadowed(t_scene *scene, int light_idx, t_vector *itx_point)
			//double *angle)
{
	double			distance;
	int				i;
	t_ray			ray;
	t_intersections	arr;
	t_intersection	*itx;

	sub_vec(&ray.dir, &scene->lights[light_idx].position, itx_point);   // Crea un raggio che va dal punto sulla superficie alla luce
	distance = vec_magnitude(&ray.dir);                                 // Calcola la distanza tra il punto e la luce
	scale_vec(&ray.dir, &ray.dir, 1 / distance);                        // Normalizza il raggio lungo 1
	ray.origin = *itx_point;                                            // Imposta il punto di partenza del raggio nel punto sulla superficie
    // aggiungi check per spot light 
	i = -1;
	arr.count = 0;                                                      // Per ogni oggetto nella scena, controlla se interseca il raggio che va verso la luce
	while (++i < scene->n_shapes)
		intersect(&scene->shapes[i], &ray, &arr);                       // Salva tutte le intersezioni trovate
	itx = hit(&arr);
	if (itx && itx->time < distance)                                    // Se c'è un'intersezione prima della luce, allora c'è un oggetto davanti , il punto è in ombra.
		return (true);
	return (false);                                                     // nessuna ombra
} 

/*
calcola il colore finale di un punto colpito da luce combinando:
luce ambientale
luce diffusa
luce riflessa
*/
t_color	phong(t_intersection *itx, t_scene *scene, int light_idx)
{
	t_phong			phong;                                                          //i vari componenti della luce
	t_color			result;                                                         //colore finale
	t_color			shape_color;	                                                //colore dell’oggetto
	const double	light_dist = vec_distance(&itx->point,                          //calcolo quanto è distante il punto dalla luce
			&scene->lights[light_idx].position);                                    // piu lontano = meno luce
	const double	attenuation = (100 * scene->lights[light_idx].intensity         //per attenuare la luce
			- light_dist) / (100 * scene->lights[light_idx].intensity - 1);
	
	shape_color = get_shape_color(itx);
	blend_colors(&phong.effective_color, &shape_color,                              //mescolo il colore dell’oggetto con il colore della luce
		&scene->lights[light_idx].color);
	if (get_specular_and_diffuse(scene, light_idx, itx, &phong) == false)           // Se il punto non è illuminato perché è in ombra, allora ritorna solo luce ambientale
		return (get_ambient(scene, shape_color));
	result = get_ambient(scene, shape_color);                                       // Altrimenti, inizia con la luce ambientale come base
	if (attenuation < 0)                                                            // Se la luce è molto distante, il punto riceve meno luce diffusa e riflessa
		return (result);                                                            // quindi le abbassiamo con l’attenuazione
	else if (attenuation > 0 && attenuation <= 1)
	{
		mult_color(&phong.diffuse, &phong.diffuse, attenuation);
		mult_color(&phong.specular, &phong.specular, attenuation);
	}
	result.r += phong.diffuse.r + phong.specular.r;                                 // Somma tutti i colori, Ambientale, Diffusa, Riflessa (speculare)
	result.g += phong.diffuse.g + phong.specular.g;
	result.b += phong.diffuse.b + phong.specular.b;
	return (result);                                                                // Ritorna il colore finale del punto sulla superficie
} 

t_color	shade_point(t_intersections *arr, t_scene *scene, t_ray *ray)
{
	t_intersection	*itx;
	int				light_idx;
	t_color			final_color;
	t_color			surface_color;
	t_color			reflected;

	itx = hit(arr);
	ft_bzero(&final_color, sizeof(t_color));
	if (itx != NULL)
	{
		prepare_computations(itx, ray);
		light_idx = 0;
		while (light_idx < scene->n_lights) // modificato 
		{
			surface_color = phong(itx, scene, light_idx);
			reflected = cast_reflection_ray(scene, itx,
					scene->settings.reflection_depth, light_idx);
			add_colors(&final_color, &final_color, &surface_color);
			add_colors(&final_color, &final_color, &reflected);
			light_idx++;
		}
	}
	return (final_color);
}


// NOTE PER ME
/*
pow è una funzione della libreria matematica del C (math.h)
Serve a fare le potenze:
pow(base, esponente)

Phong è il nome di un modello di illuminazione 3D
Quando un raggio di luce colpisce un oggetto, il colore finale del punto è fatto da 3 componenti:
Ambient = Luce "di base" che c'è ovunque, anche all’ombra
Diffuse = Luce che illumina direttamente la superficie
Specular = Luce che riflette e crea riflessi brillanti
Formula semplificata:
Colore finale = Ambient + Diffuse + Specular
*/