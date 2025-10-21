/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 18:31:44 by francesca         #+#    #+#             */
/*   Updated: 2025/10/20 13:21:25 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// *rest_out: punta al resto della riga (dopo l'id e gli spazi).
// Non modifica 'line'.
static int	check_first_token(char *line, char **rest_out, char *id)
{
	int		i;

	i = 0;
	if (rest_out)
		*rest_out = NULL;
	while (*line && ft_is_space_char(*line)) /* Skippa spazi iniziali */
		line++;
	while (*line && !ft_is_space_char(*line) && i < 9) // copia massimo 10 caratteri finché non trova spazio o fine riga
		id[i++] = *line++;
	id[i] = '\0';
	if (!is_valid_identifier(id)) /* is_valid_identifier Ritorna 1 se id è uno dei token ammessi nel mandatory */
		return (print_err_msg("Identificatore non valido"));
	while (*line && ft_is_space_char(*line))
		line++;
	if (*line)
		*rest_out = line;
	return (0);
}

// Messaggi di errore tutti dentro parse_ambient_line
static inline int handle_capital(t_scene *scene, char *line, const char *id)
{	
	if (ft_strcmp(id, "A") == 0)
			return (parse_ambient_line(scene, line) != 0); 
	else if (ft_strcmp(id, "C") == 0)
			return (parse_camera_line(scene, line) != 0);
	else if (ft_strcmp(id, "L") == 0)
			return (parse_light_line(scene, line) != 0);
	else
		return (print_err_msg("Identificatore non valido"));
}

static inline int handle_obj(t_scene *scene, char *line, const char *id)
{
	if (ft_strcmp(id, "sp") == 0)
			return (parse_sphere(scene, line) != 0);
	else if (ft_strcmp(id, "pl") == 0)
			return (parse_plane(scene, line) != 0);
	else if (ft_strcmp(id, "cy") == 0)
			return (parse_cylinder(scene, line) != 0);
	else
		return (print_err_msg("Identificatore non valido"));
}

/*
** Analizza UNA riga già letta dal .rt:
** - estrae l'id
** - aggiorna i contatori nella scena
** - controlla duplicati per A/C/L come da subject:
**   “Gli elementi definiti da lettera maiuscola 
** possono essere dichiarati una sola volta”.
** Ritorna 0 ok, 1 errore.
*/
int	lex_scan_check_and_count(t_scene *scene, char *line)
{
	char id[10];
	char *rest_out;

	if (!line || !scene)
		return (1);
	rest_out = NULL;
	if (check_first_token(line, &rest_out, id))
		return (1);
	
	if (id[0] == 'A' || id[0] == 'C' || id[0] == 'L')
		return(handle_capital(scene, rest_out, id));
	else if (id[0] == 's' || id[0] == 'p' || id[0] == 'c')
		return(handle_obj(scene, rest_out, id));
	else
		return (1);
}
