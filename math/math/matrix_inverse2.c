/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_inverse2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 18:43:53 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/14 18:22:21 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/mathRT.h"
#include <math.h>

/*
prendiamo una matrice 4x4
Per trovare il cofattore in una cella tipo [row][col]
Tagliamo la riga e la colonna della cella scelta otteniamo una matrice più piccola 3x3
Calcoliamo il determinante della matrice 3x3
Aggiungiamo un segno + o - alternato a scacchiera
Se la somma row + col è pari mettiamo -determinante
Se è dispari lasciamo il determinante così com’è
 */
static double	cofactor4(const t_mat4 *mat4, int row, int col)
{
	t_mat3	mat3;

	submat4(&mat3, mat4, row, col);
	if ((row + col) % 2 == 1)
		return (det3(&mat3));
	return (-det3(&mat3));
}

/*
restituisce il determinante di matricw 4x4
Prendiamo la prima riga della matrice
Moltiplichiamo ogni elemento della riga per il suo cofattore corrispondente
Sommiamo tutto insieme
Risultato: un numero che ci dice se la matrice è invertibile
Se determinante ≠ 0 si può invertire
Se determinante = 0 non si può invertire
 */
static double	det4(const t_mat4 *mat4)
{
	return (cofactor4(mat4, 0, 0) * (*mat4)[0][0]
			+ cofactor4(mat4, 0, 1) * (*mat4)[0][1]
			+ cofactor4(mat4, 0, 2) * (*mat4)[0][2]
			+ cofactor4(mat4, 0, 3) * (*mat4)[0][3]);
}

/*
inverte matrice 4x4, in res salvo la matrice inversa mat è la matrice da invertire
Calcoliamo il determinante di tutta la matrice.
Se il determinante è molto vicino a zero, matrice non invertibile
Calcoliamo tutti i cofattori della matrice
Li dividiamo per il determinante
Trasponiamo la matrice dei cofattori righe diventano colonne
 */
void	mat_inverse(t_mat4 *res, const t_mat4 *mat)
{
	double	det;
	int		row;

	det = det4(mat);
	if (fabs(det) < EPSILON) // Se det è molto vicino a zero, consideralo zero
		return ;
	row = 0;
	while (row < 4)
	{
		(*res)[0][row] = cofactor4(mat, row, 0) / det;
		(*res)[1][row] = cofactor4(mat, row, 1) / det;
		(*res)[2][row] = cofactor4(mat, row, 2) / det;
		(*res)[3][row] = cofactor4(mat, row, 3) / det;
		row++;
	}
}

/*
NOTE PER ME
EPSILON costante tipo 0.000001 Serve per confrontare numeri in virgola mobile senza problemi di arrotondamento
usato perche if (det == 0) può fallire per via dei decimali

fabs funzione della libreria <math.h>
Serve a prendere il valore assoluto di un numero in virgola mobile double
*/