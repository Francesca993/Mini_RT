/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transformation_matrices.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 16:16:43 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/14 18:22:13 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

void	ft_bzero(void *s, size_t n);
/*
iniziallizzazionte della matrice di traslazione 
Serve a spostare un oggetto nello spazio
“prendi ogni punto dell’oggetto e aggiungi x, y, z”
 */
void	translate_matrix(t_mat4 *mat, double x, double y, double z)
{
	ft_bzero(mat, sizeof(t_mat4));
	(*mat)[0][0] = 1;
	(*mat)[1][1] = 1;
	(*mat)[2][2] = 1;
	(*mat)[3][3] = 1;
	(*mat)[0][3] = x;
	(*mat)[1][3] = y;
	(*mat)[2][3] = z;
}

/*
iniziallizzazionte della matrice di scaling
Serve a allungare o ridurre l’oggetto lungo ogni asse
si puo schiacciare o allungare in altezza, larghezza o profondità
 */
void	scaling_matrix(t_mat4 *mat, double x, double y, double z)
{
	ft_bzero(mat, sizeof(t_mat4));
	(*mat)[0][0] = x;
	(*mat)[1][1] = y;
	(*mat)[2][2] = z;
	(*mat)[3][3] = 1;
}

/*
iniziallizzazionte della matrice di rotazione X
Ruota un oggetto attorno a un asse fisso.
X  gira una porta su e giù
Usano cos(r) e sin(r) perché i raggi formano angoli e circonferenze
angolo r in radianti
 */
void	rotation_matrix_x(t_mat4 *mat, double r)
{
	ft_bzero(mat, sizeof(t_mat4));
	(*mat)[0][0] = 1;
	(*mat)[1][1] = cos(r);
	(*mat)[1][2] = sin(r) * -1;
	(*mat)[2][1] = sin(r);
	(*mat)[2][2] = cos(r);
	(*mat)[3][3] = 1;
}

/*
iniziallizzazionte della matrice di rotazione Y
Ruota un oggetto attorno a un asse fisso.
Y  gira una ruota di bicicletta avanti/indietro
Usano cos(r) e sin(r) perché i raggi formano angoli e circonferenze
angolo r in radianti
 */
void	rotation_matrix_y(t_mat4 *mat, double r)
{
	ft_bzero(mat, sizeof(t_mat4));
	(*mat)[0][0] = cos(r);
	(*mat)[0][2] = sin(r);
	(*mat)[1][1] = 1;
	(*mat)[2][0] = sin(r) * -1;
	(*mat)[2][2] = cos(r);
	(*mat)[3][3] = 1;
}

/*
iniziallizzazionte della matrice di rotazione 
Serve a ruotare un oggetto attorno a un asse a scelta
La formula usa cos(angle), sin(angle) e le coordinate dell’asse ax->x/y/z 
per calcolare tutti i punti dopo la rotazione
*/

void	axis_angle(t_mat4 *rot_mat, const t_vector *ax, double angle)
{
	(*rot_mat)[0][0] = cos(angle) + ax->x * ax->x * (1 - cos(angle));
	(*rot_mat)[0][1] = ax->x * ax->y * (1 - cos(angle)) - ax->z * sin(angle);
	(*rot_mat)[0][2] = ax->x * ax->z * (1 - cos(angle)) + ax->y * sin(angle);
	(*rot_mat)[0][3] = 0;
	(*rot_mat)[1][0] = ax->x * ax->y * (1 - cos(angle)) + ax->z * sin(angle);
	(*rot_mat)[1][1] = cos(angle) + ax->y * ax->y * (1 - cos(angle));
	(*rot_mat)[1][2] = ax->y * ax->z * (1 - cos(angle)) - ax->x * sin(angle);
	(*rot_mat)[1][3] = 0;
	(*rot_mat)[2][0] = ax->z * ax->x * (1 - cos(angle)) - ax->y * sin(angle);
	(*rot_mat)[2][1] = ax->z * ax->y * (1 - cos(angle)) + ax->x * sin(angle);
	(*rot_mat)[2][2] = cos(angle) + ax->z * ax->z * (1 - cos(angle));
	(*rot_mat)[2][3] = 0;
	(*rot_mat)[3][0] = 0;
	(*rot_mat)[3][1] = 0;
	(*rot_mat)[3][2] = 0;
	(*rot_mat)[3][3] = 1;
}
/*
NOTE PER ME
Sono tutte matrici 4x4 (t_mat4) → una tabella 4 righe x 4 colonne
L’ultima colonna [0..3][3] serve a traslazioni e trasformazioni omogenee
Quando moltiplichi un vettore (x,y,z,w) per la matrice:
Translate → sposta il punto
Scale → allunga/riduce
Rotate → ruota
Axis-angle → ruota attorno a qualsiasi direzione

Regola generale per capire le matrici:
Ultima colonna [0..2][3] → traslazione.
Diagonal principale [0..2][0..2] → scaling se sono numeri >0, rotazione se contengono cos/sin.
Ultima riga [3][0..3] → sempre [0,0,0,1] per le trasformazioni 3D.
*/