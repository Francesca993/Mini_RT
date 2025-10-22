/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:28:42 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/10/13 18:05:49 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

/*
questa funzione costruisce la camera nello spazio 3D pronta per generare raggi
Prepara i vettori della camera
from = posizione della camera nello spazio
dir = direzione in cui guarda la camera, normalizzata a lunghezza 1
Definisce il vettore “up” dello spazio, per calcolare l’orientamento della camera correttamente
Combina posizione e orientamento in una matrice 4x4
Calcola l’inversa, per trasformare i raggi dal mondo nello spazio locale della camera
*/
void	calculate_camera_transform(t_scene *scene)
{
	t_vector		from;
	t_vector		up;

	from.x = scene->cam.pos.x;
	from.y = scene->cam.pos.y;
	from.z = scene->cam.pos.z;
	from.w = 1;
	normalize_vec(&scene->cam.dir);
	up.x = 0;
	up.y = 1;
	up.z = 0;
	up.w = 0;
	view_transform(&scene->cam.transform, &from, &up,
		&scene->cam.dir);
	mat_inverse(&scene->cam.inv_trans, &scene->cam.transform);
}

/*
ruota il mondo secondo la direzione della camera, 
trasla il mondo per mettere la camera all’origine
così quando generiamo i raggi, possiamo partire sempre dalla posizione (0,0,0) nello spazio locale della camera
Normalizza il vettore up, calcola il vettore left per costruire la base della matrice,
il true up assicura che il vettore verso l’alto sia perfettamente ortogonale a forward e left
costruisce la matrice di orientamento per far guardare la camera nella direzione giusta, poi 
la matrice di traslazione in modo che la camera sia all’origine e le COMBINA 
*/
void	view_transform(t_mat4 *res, const t_vector *from, const t_vector *up,
	const t_vector *forward)
{
	t_vector	left;
	t_vector	true_up;
	t_vector	upn;
	t_mat4		orientation;
	t_mat4		translation;

	upn = *up;
	normalize_vec(&upn);
	cross_product(&left, forward, &upn);
	normalize_vec(&left);
	cross_product(&true_up, &left, forward);
	normalize_vec(&true_up);
	ft_bzero(&orientation, sizeof(t_mat4));
	orientation[0][0] = left.x;
	orientation[0][1] = left.y;
	orientation[0][2] = left.z;
	orientation[1][0] = true_up.x;
	orientation[1][1] = true_up.y;
	orientation[1][2] = true_up.z;
	orientation[2][0] = forward->x * -1;
	orientation[2][1] = forward->y * -1;
	orientation[2][2] = forward->z * -1;
	orientation[3][3] = 1;
	translate_matrix(&translation, from->x * -1, from->y * -1, from->z * -1);
	mat_multiply(res, &orientation, &translation);
}
/*
prepara tutto il necessario per generare i raggi pixel-per-pixel
Calcola dimensioni viewport
Calcola “half view”, dalla FOV ricava metà larghezza dell’angolo in coordinate spaziali
gestione dell'aspect ratio per evitare deformazioni
calcola la dimensione di un pixel nello spazio della scena, 
per sapere quanto spostarsi nello spazio 3D per passare da un pixel all’altro
*/
void	camera_init(t_camera *camera, t_scene *scene)
{
	double	half_view;
	double	aspect;
	double	h;
	double	w;

	h = scene->settings.render_h;
	w = scene->settings.render_w;
	if (scene->settings.edit_mode == true)
	{
		h = scene->settings.edit_h;
		w = scene->settings.edit_w;
	}
	half_view = tan((camera->fov / 2.0f) * M_PI / 180.0f);
	aspect = w / h;
	if (aspect >= 1)
	{
		camera->half_width = half_view;
		camera->half_height = half_view / aspect;
	}
	else
	{
		camera->half_width = half_view * aspect;
		camera->half_height = half_view;
	}
	camera->pixel_size = (camera->half_width * 2) / w;
}

