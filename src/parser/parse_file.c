/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:48:43 by francesca         #+#    #+#             */
/*   Updated: 2025/10/21 23:54:31 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* ---- HELPER ---- */
/*
Quello che fa è creare una variabile c di tipo  t_vector,
assegnargli i valori r, g, b ai rispettivi campi, e restituirla.
*/
static inline t_vector	vector_make(double x, double y, double z)
{
	t_vector	v;

	v.x = x;
	v.y = y;
	v.z = z;
	v.w = 0.0; /* default: vettore direzione */
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

/* Inizializza/azzera completamente la scena,
liberando eventuale storage precedente. */
void	scene_reset(t_scene *scene)
{
	// /* --- sicurezza memoria dinamica oggetti --- */
	    /* 1) pulizia lista precedente, se esiste */
    objlist_destroy(scene);
    /* 2) init lista vuota + azzera contatori */
    objlist_init(scene);
	/* --- Ambient --- */
	scene->ambient.intensity = 0.0;
	scene->ambient.color = color_make(0.0, 0.0, 0.0, 0.0);
	scene->ambient.present = false;
	/* --- Camera --- */
	scene->cam.pos = vector_make(0.0, 0.0, 0.0);
	scene->cam.dir = vector_make(0.0, 0.0, 0.0);
	/* direzione di default “in avanti” */
	scene->cam.fov = 0.0;
	scene->cam.present = false;
	/* --- Light (spot principale del mandatory) --- */
	scene->lights->position = vector_make(0.0, 0.0, 0.0);
	scene->lights->intensity = 0.0;
	scene->lights->color = color_make(0.0, 0.0, 0.0, 0.0);
	scene->lights->present = false;
	/* --- contatori logici per validazione (A/C/L una sola volta) --- */
	scene->n_ambient = 0;
	scene->n_camera = 0;
	scene->n_lights = 0;
	scene->n_shapes = 0;
	/* --- contatori oggetti geometrici --- */
	scene->n_spheres = 0;
	scene->n_planes = 0;
	scene->n_cylinders = 0;
	return ;
}

/* Rimuove New_line finale */
void	chop_newline(char *s)

{
	size_t n;

	if (!s)
		return ;
	n = ft_strlen(s);
	if (n > 0 && s[n - 1] == '\n')
		s[n - 1] = '\0';
	return ;
}

/*Prende la riga con gnl, salta gia quelle vuote, inizializza scene,
leva il \n e mette '\0' e fa partire la funzione che mentre legge riga
per irga, fa anche il chekc in "tempo reale" dei requisiti. */
int	parse_file(const char *path, t_scene *scene)
{
	char	*line;
	int		fd;

	if (!path || !scene)
		return (print_err_msg("parse_file: argomenti non validi."));
	scene_reset(scene); // reset contatori / inizializzazione scena
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (print_err_msg("miniRT: open"));
	while ((line = get_next_line(fd)) != NULL)
	{
		if (ft_is_blank(line)) // <-- salta riga vuota o spazi
		{
			free(line);
			continue ; // salta le righe vuote
		}
		chop_newline(line); // Rimuove New line finale e mette '0'
		/*-------CONTROLLI DIRETTAMENTE SULLA LINEA APPENA LETTA -----------*/
		if (lex_scan_check_and_count(scene, line) != 0)
		{
			free(line);
			close(fd);
			return (1);
		}
		free(line);
	}
	if (close(fd) == -1)
		return (print_err_msg("Error: miniRT: close"));
	if (check_presence(scene) == 1)
		return (1);
	return (0);
}
