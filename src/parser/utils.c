/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:25:56 by francesca         #+#    #+#             */
/*   Updated: 2025/10/20 16:57:26 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* helper interno: consuma una virgola con spazi opzionali attorno e ritorna 0 se non trova la virgola*/
int	skip_comma(const char **input_ptr)
{
	const char	*cursor;

	if (input_ptr == NULL || *input_ptr == NULL)
		return (0);
	cursor = *input_ptr;
	if (*cursor != ',')
		return (0);
	cursor++;
	*input_ptr = cursor;
	return (1);
}
/* Stampa messaggio di errore */
int	print_err_msg(const char *msg)
{
	fprintf(stderr, "Error\n"); /* richiesto dal subject */
	fprintf(stderr, "%s\n", msg);
	return (1);
}

/* Stampa messaggio di errore più specifico */
int	err_msg(char *type, int num, const char *msg)
{
	// fprintf(stderr, "Error\n"); /* richiesto dal subject */
	fprintf(stderr, "%s numero: %d, %s\n", type, num, msg);
	return (1);
}
