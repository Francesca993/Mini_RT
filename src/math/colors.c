/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:54:23 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/17 09:54:59 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

/*
 somma due colori il risultato in res, c1 è il primo colore  c2 il secondo
 */
void	add_colors(t_color *res, const t_color *c1, const t_color *c2)
{
	res->r = c1->r + c2->r;
	res->g = c1->g + c2->g;
	res->b = c1->b + c2->b;
}

/*
 Moltiplica il colore per lo scalare 
 risultato salvato in res
 color è il colore che verra moltiplicato
 val è il valore usato per moltiplicare il colore, 
 Utile per schiarire o scurire un colore
 */
void	mult_color(t_color *res, const t_color *color, double val)
{
	res->r = color->r * val;
	res->g = color->g * val;
	res->b = color->b * val;
}

/*
 Sottrae un colore dall'altro
 il risultato in res, c1 è il primo colore  c2 il secondo 
 */
void	sub_colors(t_color *res, const t_color *c1, const t_color *c2)
{
	res->r = c1->r - c2->r;
	res->g = c1->g - c2->g;
	res->b = c1->b - c2->b;
}

/*
Mischia due colori insiweme moltiplicando i loro componenti
il risultato in res, c1 è il primo colore  c2 il secondo
 */
void	blend_colors(t_color *res, const t_color *c1, const t_color *c2)
{
	res->r = c1->r * c2->r;
	res->g = c1->g * c2->g;
	res->b = c1->b * c2->b;
}
