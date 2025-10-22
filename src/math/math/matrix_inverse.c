/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_inverse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 17:39:10 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/14 18:22:25 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

/*
restituisce la determinante della matrice 2x2
Serve come “mattoncino base” per costruire determinanti più grandi (3×3, 4×4)
 */
static double	det2(const t_mat2 *mat)
{
	return ((*mat)[0][0] * (*mat)[1][1] - (*mat)[0][1] * (*mat)[1][0]);
}

/*
Questa funzione prende una matrice 3×3 e toglie:
la riga row
la colonna col
Per ottenere una piccola matrice 2×2 salvata in res
 */
static void	submat3(t_mat2 *res, const t_mat3 *mat, int row, int col)
{
	int	i;
	int	j;
	int	i_sub;
	int	j_sub;

	i = -1;
	i_sub = 0;
	while (++i < 3)
	{
		if (i == row)
			continue ;
		j_sub = 0;
		j = 0;
		while (j < 3)
		{
			if (j == col)
				j++;
			else
				(*res)[i_sub][j_sub++] = (*mat)[i][j++];
		}
		i_sub++;
	}
}

/*
Stesso concetto, ma qui 4×4 e togli una riga e una colonna per ottenere una 3×3
Serve per calcolare cofattori e determinanti di matrici 4×4 da usare per l'inversa 
 */
void	submat4(t_mat3 *res, const t_mat4 *mat, int row, int col)
{
	int	i;
	int	j;
	int	i_sub;
	int	j_sub;

	i = -1;
	i_sub = 0;
	while (++i < 4)
	{
		if (i == row)
			continue ;
		j_sub = 0;
		j = 0;
		while (j < 4)
		{
			if (j == col)
				j++;
			else
				(*res)[i_sub][j_sub++] = (*mat)[i][j++];
		}
		i_sub++;
	}
}

/*
Un cofattore è il determinante della sotto-matrice, con un segno alternato a scacchiera
Prendi la 2×2 calcoli il determinante
Applichi segno + o − in base a posizione (row+col pari o dispari)
 */
static double	cofactor3(const t_mat3 *mat3, int row, int col)
{
	t_mat2	mat2;

	submat3(&mat2, mat3, row, col);
	if ((row + col) % 2 == 1)
		return (det2(&mat2));
	return (-det2(&mat2));
}

/**
 Questa funzione serve perché per invertire una matrice 4×4, ho bisogno di tanti determinanti 3×3
 restituisce la determinante
 */
double	det3(const t_mat3 *mat3)
{
	return (cofactor3(mat3, 0, 0) * (*mat3)[0][0]
			+ cofactor3(mat3, 0, 1) * (*mat3)[0][1]
			+ cofactor3(mat3, 0, 2) * (*mat3)[0][2]);
}
/*
NOTE PER ME
L’inversa della matrice serve per “tornare al punto di partenza” → annullare traslazioni, rotazioni e scalamenti

il determinante dice:
di quanto si “deforma” lo spazio (scalatura globale),
se la trasformazione inverte l’orientamento (det negativo = riflessione)

Se det = 1  la trasformazione conserva volume/orientamento (come rotazioni)
Se det = 2  raddoppia il volume
Se det = 0  la matrice non è invertibile (perde dimensioni — tipo schiacciare un cubo in un piano)

Determinante Quanto una matrice “deforma” lo spazio — se è invertibile
Cofattore Determinante locale × segno +/− “a scacchiera”
Inversa È come il “tasto indietro” di una trasformazione
*/