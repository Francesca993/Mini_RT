/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:58:11 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/14 18:22:18 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

void	*ft_memset(void *s, int c, size_t n);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t len);

/*
Moltiplica matrice m1 con matrice m2 e salva il reisultato della nuova matrice in res
combina due trasformazioni in una sola
Ogni cella della nuova matrice (temp[row][col]) 
è calcolata come una somma di prodotti tra riga di m1 e colonna di m2
*/
void	mat_multiply(t_mat4 *res, const t_mat4 *m1, const t_mat4 *m2)
{
	unsigned char	row;
	t_mat4			temp;

	row = 0;
	while (row < 4)
	{
		temp[row][0] = (*m1)[row][0] * (*m2)[0][0] \
			+ (*m1)[row][1] * (*m2)[1][0]
			+ (*m1)[row][2] * (*m2)[2][0]
			+ (*m1)[row][3] * (*m2)[3][0];
		temp[row][1] = (*m1)[row][0] * (*m2)[0][1] \
			+ (*m1)[row][1] * (*m2)[1][1]
			+ (*m1)[row][2] * (*m2)[2][1]
			+ (*m1)[row][3] * (*m2)[3][1];
		temp[row][2] = (*m1)[row][0] * (*m2)[0][2] \
			+ (*m1)[row][1] * (*m2)[1][2]
			+ (*m1)[row][2] * (*m2)[2][2]
			+ (*m1)[row][3] * (*m2)[3][2];
		temp[row][3] = (*m1)[row][0] * (*m2)[0][3] \
			+ (*m1)[row][1] * (*m2)[1][3]
			+ (*m1)[row][2] * (*m2)[2][3]
			+ (*m1)[row][3] * (*m2)[3][3];
		row++;
	}
	ft_memcpy(res, &temp, sizeof(t_mat4));
}

/*
Moltiplica la matrice per un vettore slava il risultato in res
Questa funzione prende una matrice (mat) e un punto o vettore (vec) che devono essere moltiplicati 
e ti restituisce il punto trasformato (res)
Formula: somma dei prodotti tra riga della matrice e componenti del vettore.
Se la matrice è di traslazione, il punto si sposta
Se è di scaling, il punto si allunga o schiaccia
Se è di rotazione, il punto gira
 */
void	mat_vec_multiply(t_vector *res, const t_mat4 *mat,
			const t_vector *vec)
{
	res->x = vec->x * (*mat)[0][0] + vec->y * (*mat)[0][1] \
	+ vec->z * (*mat)[0][2] + vec->w * (*mat)[0][3];
	res->y = vec->x * (*mat)[1][0] + vec->y * (*mat)[1][1] \
	+ vec->z * (*mat)[1][2] + vec->w * (*mat)[1][3];
	res->z = vec->x * (*mat)[2][0] + vec->y * (*mat)[2][1] \
	+ vec->z * (*mat)[2][2] + vec->w * (*mat)[2][3];
	res->w = vec->x * (*mat)[3][0] + vec->y * (*mat)[3][1] \
	+ vec->z * (*mat)[3][2] + vec->w * (*mat)[3][3];
}

/*
È la matrice che non cambia nulla
Diagonale principale = 1, tutto il resto = 0
utile come base per costruire altre trasformazioni
 */
void	identity_matrix(t_mat4 *mat)
{
	ft_bzero(mat, sizeof(t_mat4));
	(*mat)[0][0] = 1;
	(*mat)[1][1] = 1;
	(*mat)[2][2] = 1;
	(*mat)[3][3] = 1;
}

/*
Scambia righe e colonne della matrice
trasporre significa girare la tabella lungo la diagonale
Serve per operazioni matematiche, come trovare l’inversa
 */
void	transpose_matrix(t_mat4 *mat)
{
	double			temp;
	unsigned char	row;
	unsigned char	col;

	row = 1;
	while (row < 4)
	{
		col = 0;
		while (col < row)
		{
			temp = (*mat)[row][col];
			(*mat)[row][col] = (*mat)[col][row];
			(*mat)[col][row] = temp;
			col++;
		}
	row++;
	}
}
