/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 11:26:54 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/20 14:11:58 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"


// L'intersezione della sfera non necessita del raggio per essere trasformato
// ray: raggio in world space (origine + direzione)
// xs: struttura dove appendere le intersezioni trovate
// sphere: puntatore allo shape

/*
Funzione per vedere se un raggio colpisce una sfera, usando formula quadrica
Calcola il discriminante h:
Se h < 0 → non colpisce.
Se h >= 0 → due punti di intersezione: -b ± sqrt(h)
*/
bool	intersect_sphere_fast(const t_ray *ray, t_intersections *xs,
	t_shape *sphere)
{
	t_vector	oc; // vector da centro sfera C a origine raggio O (O - C)
	double		b;  // sono variabili usate per la formula quadratica / discriminante
	double		c;
	double		h;

	sub_vec(&oc, &ray->origin, &sphere->origin); 				// prende il vettore che va dal centro della sfera al punto di partenza del raggio. (Questa è la O - C nella matematica).
	b = dot_product(&oc, &ray->dir);             				// (prodotto scalare tra oc e la direzione del raggio D) , ci dice a che distanza il raggio colpisce
	c = dot_product(&oc, &oc) - sphere->props.radius_squared;   // è la distanza al quadrato tra origine raggio e centro sfera, ci dice quanto il raggio è “lontano” dal centro
	h = b * b - c;												// Formula semplificata del discriminante
	if (h < 0.0)									            // se il discriminante è negativo → nessuna soluzione reale → il raggio non incontra la sfera 
		return (false);								            // Funzione ritorna false
	h = sqrt(h);									            //Calcola la radice quadrata del discriminante serve per trovare i due tempi di intersezione lungo il raggio
	xs->arr[xs->count].time = -b - h;							// il punto dove il raggio entra nella sfera
	xs->arr[xs->count].shape = sphere;
	xs->arr[xs->count + 1].time = -b + h;						// punto dove il raggio esce dalla sfera
	xs->arr[xs->count + 1].shape = sphere;
	xs->count += 2;												// aggiungiamo 2 per entrata e uscita
	return (true);
}
/*
Questa funzione calcola l'intersezione raggio–piano
denom = D · N (direzione del raggio per la normale del piano)
Se denom è molto vicino a 0 il raggio è parallelo al piano → nessuna intersezione utile, 
ritorna false. La soglia 0.00001 evita divisione per valori molto piccoli (problemi numerici)
*/
bool	intersect_plane_fast(const t_ray *ray, t_shape *plane,
		t_intersections *xs)
{
	double	denom;																	// prodotto scalare tra direzione del raggio e la normale del piano

	denom = dot_product(&ray->dir, &plane->orientation);							// dice quanto il raggio è inclinato rispetto al piano
	if (fabs(denom) < 0.00001)														// Se denom ≈ 0 → il raggio è parallelo al piano → non lo colpisce, fabs rende il valore assoluto
		return (false);
	xs->arr[xs->count].time = -(dot_product(&ray->origin, &plane->orientation)      // Si calcola il valore t → cioè quando il raggio colpisce il piano,
		- plane->props.distance_from_origin) / denom;								// e si salva l’intersezione in xs 
	xs->arr[xs->count].shape = plane;
	xs->count++;
	return (true);
}
/*
controlla che tipo di oggetto è, e chiama la funzione giusta
Funzione dispatcher che decide quale routine di intersezione chiamare in base al tipo di shape.
Prima cosa: se xs è pieno (>=200) non aggiunge altro e ritorna false per sicurezza.

Se lo shape è una sfera o un piano, chiama le versioni “fast” che non trasformano il raggio: 
assumono che la sfera/piano siano espressi in world space
*/
bool	intersect(t_shape *shape, const t_ray *ray, t_intersections *xs)
{
	if (xs->count >= 200)														// Se hai già trovato 200 intersezioni l’array è pieno, blocchi
		return (false);
	if (shape->type == SPHERE)
		return (intersect_sphere_fast(ray, xs, shape));
	if (shape->type == PLANE)
		return (intersect_plane_fast(ray, shape, xs));
	return (false);
}


//NOTE
/*
FORMULA QUADRICA
t = (-b ± √(b² - 4ac)) / 2a
E il pezzo importante è Δ = b² - 4ac, che si chiama discriminante.
*/ 