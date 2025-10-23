/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 19:11:03 by francesca         #+#    #+#             */
/*   Updated: 2025/10/23 08:36:02 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

/* ---- HELPER ---- */
/*
Quello che fa è creare una variabile c di tipo  t_vector,
assegnargli i valori r, g, b ai rispettivi campi, e restituirla.
*/
static inline t_vector	vector_make(double x, double y, double z, double w)
{
	t_vector	v;

	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return (v);
}
/*
Quello che fa è creare una variabile c di tipo t_color, assegnargli i valori r,
g, b ai rispettivi campi, e restituirla.
*/
static inline t_color	color_make(double r, double g, double b, double t)
{
	t_color	c;

	c.r = r;
	c.g = g;
	c.b = b;
	c.a = t;
	return (c);
}

static inline void	camera_make(t_scene *scene)
{
	/* posizione (punto) w=1.0, direzione (vettore) w=0.0 */
	scene->cam.pos = vector_make(0.0, 0.0, 0.0, 0.0);
	scene->cam.dir = vector_make(0.0, 0.0, 0.0, 0.0);
	/* direzione di default “in avanti” */
	scene->cam.fov = 0.0;
	scene->cam.fov_deg = 0;
	scene->cam.present = false;
	scene->cam.pixel_size = 0.0;
	scene->cam.half_width = 0.0;
	scene->cam.half_height = 0.0;
	/* azzera le matrici transform/inv_trans */
	ft_bzero(&scene->cam.transform, sizeof(scene->cam.transform));
	ft_bzero(&scene->cam.inv_trans, sizeof(scene->cam.inv_trans));
}

static inline void settings_make(t_scene *scene)
{
	scene->settings.render_w = 0;   // larghezza finale dell'immagine renderizzata
	scene->settings.render_h = 0;  // altezza finale dell'immagine renderizzata
	scene->settings.edit_w = 0;    // larghezza in modalità "edit" risoluzione ridotta, usata per debug
	scene->settings.edit_h = 0;     // altezza in modalità "edit" risoluzione ridotta, usata per debug
	// scene->settings.edit_mode = false; // se true → usa edit_w/edit_h “render veloce” invece che render_w/render_h quello finale (false)
	// 				// flag letto dentro camera_init per scegliere quale risoluzione usare

	scene->settings.reflection_depth = 0;
}

static inline int light_init(t_scene *scene)
{
    /* libera eventuale blocco precedente */
    if (scene->lights) {
        free(scene->lights);
        scene->lights = NULL;
    }

    /* alloca array di 1 luce */
    scene->lights = ft_calloc(1, sizeof *scene->lights); // sizeof(t_light)
    if (!scene->lights)
        return (print_err_msg("allocazione luce fallita"));

    /* inizializza i campi della luce [0] */
    scene->lights[0].position  = vector_make(0.0, 0.0, 0.0, 0.0);
    scene->lights[0].intensity = 0.0;
    scene->lights[0].color     = color_make(0.0, 0.0, 0.0, 0.0);
    scene->lights[0].present   = false;
    return 0;
}

/* Inizializza/azzera completamente la scena,
liberando eventuale storage precedente. */
int	scene_reset(t_scene *scene)
{
	// /* --- sicurezza memoria dinamica oggetti --- */
	    /* 1) pulizia lista precedente, se esiste */
    objlist_destroy(scene);
    /* 2) init lista vuota + azzera TUTTI I contatori */
    objlist_init(scene);
	/* --- Ambient --- */
	scene->ambient.intensity = 0.0;
	scene->ambient.color = color_make(0.0, 0.0, 0.0, 0.0);
	scene->ambient.present = false;
	/* --- Camera --- */
	camera_make(scene);
    if (light_init(scene) == 1)
        return (0);
	settings_make(scene);
	
	return (1);
}
