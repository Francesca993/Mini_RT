/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_arithmetic.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:39:29 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/14 18:22:01 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

/*
Somma due vettori e salva il risultato in res
v1(1,2,3,1) + v2(4,0,-1,0) = res(5,2,2,1)
Serve per spostare punti o combinare direzioni.
*/
void	add_vec(t_vector *res, const t_vector *v1, const t_vector *v2)
{
	res->x = v1->x + v2->x;
	res->y = v1->y + v2->y;
	res->z = v1->z + v2->z;
	res->w = v1->w + v2->w;
}

/*
sottrae due vettori e salva il risultato in res
(4,5,6,1) - (1,2,3,1) = (3,3,3,0)
Serve per calcolare la direzione da un punto a un altro e
ottenere “freccia tra due punti” per distanza o raggio
 */
void	sub_vec(t_vector *res, const t_vector *v1, const t_vector *v2)
{
	res->x = v1->x - v2->x;
	res->y = v1->y - v2->y;
	res->z = v1->z - v2->z;
	res->w = v1->w - v2->w;
}

/*
Allunga o accorcia la freccia
(1,2,0,0) * 3 = (3,6,0,0)
il risultato di scale * v viene salvato nel vettore res
Se scale = -1 la freccia punta nella direzione opposta (utile per riflessioni)
 */
void	scale_vec(t_vector *res, const t_vector *v, double scale)
{
	res->x = scale * v->x;
	res->y = scale * v->y;
	res->z = scale * v->z;
	res->w = scale * v->w;
}

/*
Inverte completamente la direzione della freccia.
(1,0,0,0) diventa (-1,0,0,0)
Serve per fare riflessioni, invertire normali, cambiare direzioni dei raggi
 */
void	negate_vec(t_vector *res, const t_vector *v)
{
	res->x = -1 * v->x;
	res->y = -1 * v->y;
	res->z = -1 * v->z;
	res->w = -1 * v->w;
}
/*
funzione chiave per i riflessi speculari
res = il raggio riflesso
in_vector = colpisce una superficie con una normale (normal)
Formula matematica del riflesso
r=v−2⋅(v⋅n)⋅n
Dove:
v = raggio incidente (in_vector)
n = normale della superficie (normal)
r = raggio riflesso (res)
v⋅n = dot product → quanto il raggio “punta verso la superficie”
otteniamo la direzione riflessa finale
*/
void	reflect_vector(t_vector *res, t_vector *in_vector, t_vector *normal)
{
	scale_vec(res, normal, dot_product(in_vector, normal) * 2);
	sub_vec(res, in_vector, res);
}
/*
NOTA PER ME 
Se w = 1 (punto) e fai add_vec , sposti il punto nello spazio.
Se w = 0 (direzione) e fai scale_vec , cambi solo la lunghezza della freccia, non il punto
*/