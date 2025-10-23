/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:24:06 by fmontini          #+#    #+#             */
/*   Updated: 2025/10/22 21:00:55 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include "../Libft/libft.h"
//# include "../mlx/mlx.h"
# include "miniRT_struct.h"
# include <math.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <limits.h>
# include "mathematical.h"


/* ================= Hook ================= */
int			close_hook(void *param);
int			key_hook(int keycode, void *param);

/* ================= Utils ================= */
void		print_usage(const char *prog);
void	    chop_newline(char *s);      // Rimuove /n finale
void	    scene_free(t_scene *scene); // Libera la Union,gli objects della t_scene
int			parse_double(const char **line, double *out);
int			parse_rgb(const char **input_ptr, t_color *out_color);
int			parse_vec3(const char **input_ptr, t_vector *out_vec, double normalized);
int			skip_comma(const char **input_ptr);
int			parse_int(const char **line_pointer, int *out_value); // Converte un intero, 
					// ma restituisce anche il puntatore a dopo il numero convertito

/* ================= Debug ================= */
void		debug_print_scene(const t_scene *scene_ptr, const char *title);

/* ================= Free ================= */
void		free_array(char **arr);
int			print_err_msg(const char *msg);
int			err_msg(char *type, int num, const char *msg);
int			scene_reset(t_scene *scene);

/* ================= ft_skip_spaces ================= */
int			ft_is_blank(const char *s);
int			ft_is_space_char(char c);
const char	*skip_spaces(const char *p);

/* ================= Check File ================= */
int			has_rt_extension(const char *path);  // controlla che abbia l'estensione .rt
int			check_startingscene(t_scene *scene); // controlla che cia sia una sola A, L,
                                        // C e almeno una sp, cy, pl
int			is_valid_identifier(const char *id);  // Ritorna 1 se id è uno 
											// dei token ammessi nel mandatory
int 		check_vec3direction(t_vector *direction_value);
int			check_presence(t_scene *scene); // Controlla presenze obbligatorie a fine file

/* ================= Parse File ================= */
int			parse_file(const char *path, t_scene *scene);
int			parse_ambient_line(t_scene *scene, char *rest_of_line);
int			parse_camera_line(t_scene *scene, char *rest_of_line);
int			parse_light_line(t_scene *scene, char *rest_of_line);
int			parse_sphere(t_scene *scene, char *rest_of_line);
int			parse_plane(t_scene *scene, char *rest_of_line);
int			parse_cylinder(t_scene *scene, char *rest_of_line);

/* ================= Lexer ================= */
int			lex_scan_check_and_count(t_scene *scene, char *line);
		///* Prima scansione della matrice e aggiornamento contatori
		//+ duplicati A/C/L */

/* ================= Linked List Utils ================= */
void objlist_init(t_scene *scene);
void objlist_destroy(t_scene *scene);
t_objnode *objnode_new(t_shape_type type, t_figures fig);
int object_list_append(t_scene *scene, t_shape_type object_type, t_figures object_payload);

#endif
