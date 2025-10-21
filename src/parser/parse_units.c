/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_units.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 13:46:16 by francesca         #+#    #+#             */
/*   Updated: 2025/10/20 13:02:04 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"


/* Parsing double base-10 con '.' (niente expo). Ritorna 1 ok, 0 errore. */
/*
Salta gli spazi iniziali.
Legge un segno opzionale + o -.
Legge la parte intera (sequenza di cifre).
Se c’è un punto . legge la parte frazionaria (altre cifre).
Se non ha letto nessuna cifra (né prima né dopo il .) → fallisce.
Calcola		valore = segno * (intero + frazionale) dove
frazionale = frac / scale (es. per “.37” → frac=37, scale=100, quindi 0.37).
Aggiorna il puntatore d’ingresso per “consumare” i caratteri letti.
Ritorna 1 se ok, 0 se errore.
*/
int	parse_double(const char **line, double *out)
{
	const char	*n_line;
	int			sign;

	double val; // Accumula la parte intera (prima del .). Se leggi “123”,
				// alla fine val = 123.
	double frac; // Accumulano la parte frazionaria come intero e la scala corrispondente.
	double scale; // La scala o.100 / 0.10 / 0.1000
	int ndigits; // Conta quante cifre sono state lette prima il punto.
	int nfrac; // Conta quante cifre sono state lette dopo il punto.
	double fractional_part; // variabile temporanea per calcolare frac/scale
	n_line = skip_spaces(*line); // Salta gli spazi iniziali
	sign = 1;
	val = 0.0;
	frac = 0.0;
	scale = 1.0;
	ndigits = 0;
	nfrac = 0;
	if (*n_line == '+' || *n_line == '-') 	/* Se c’è +/-, aggiorna sign e avanza. */
	{
		if (*n_line == '-')
			sign = -1;
		n_line++;
	}
	while (ft_isdigit(*n_line)) /* Parte intera */
	{
		val = val * 10.0 + (double)(*n_line - '0');
		n_line++;
		ndigits++;
	}
	/* Parte dopo il . */
	if (*n_line == '.')
	{
		n_line++;
		while (ft_isdigit(*n_line))
		{
			frac = frac * 10.0 + (double)(*n_line - '0');
			scale *= 10.0;
			n_line++;
			nfrac++;
		}
	}
	/* Se non è stato letto nessun carattere numerico né nella parte intera né in quella frazionaria,
		la funzione fallisce e restituisce 0. */
	if (ndigits == 0 && nfrac == 0)
		return (0);
	// /* Calcolo esplicito della parte frazionaria normalizzata. Se nfrac è zero uso 0.0,
	// 	altrimenti uso frac / scale. */
	if (nfrac == 0)
		fractional_part = 0.0;
	else
		fractional_part = frac / scale;
	*out = sign * (val + fractional_part); /* Assegno al puntatore di output il valore con segno */
	*line = n_line; /* Aggiorno il puntatore della stringa chiamante per avanzare alla nuova posizione */
	return (1);
}

/* Parso "x,y,z" senza spazi, usando parse_double, se int è 1 allora imposta w a 1 sennò a 0 */
int	parse_vec3(const char **input_ptr, t_vector *out_vec, double normalized)
{
	const char	*cursor;
	double		x;
	double		y;
	double		z;

	if (input_ptr == NULL || *input_ptr == NULL || out_vec == NULL)
		return (0);
	cursor = *input_ptr;
	if (!parse_double(&cursor, &x))
		return (0);
	if (!skip_comma(&cursor))
		return (0);
	if (!parse_double(&cursor, &y))
		return (0);
	if (!skip_comma(&cursor))
		return (0);
	if (!parse_double(&cursor, &z))
		return (0);
	out_vec->x = x;
	out_vec->y = y;
	out_vec->z = z;
	out_vec->w = normalized;
	*input_ptr = cursor;
	return (1);
}

/* vieta spazi PRIMA del numero: il cursore deve essere già sulla prima cifra */
static int	parse_RGB_component(const char **input_ptr, int *out_value)
{
	const char	*cursor;
	int			value;
	int			digit;

	if (input_ptr == NULL || *input_ptr == NULL || out_value == NULL)
		return (0);
	cursor = *input_ptr;
	if (*cursor == '+' || *cursor == '-' || !ft_isdigit(*cursor)) /* segni non ammessi e NESSUNO spazio prima: deve esserci SUBITO una cifra */
		return (0);
	value = 0;
	while (ft_isdigit(*cursor)) /* leggi cifre contigue (niente spazi dentro) e valida on-the-fly */
	{
		digit = (int)(*cursor - '0');
		value = value * 10 + digit;
		if (value > 255) /* supera 255 → errore immediato */
			return (0);
		cursor++;
	}
	*out_value = value;
	*input_ptr = cursor;
	return (1);
}

int	parse_rgb(const char **input_ptr, t_color *out_color)
{
	const char	*cursor; 
	int			r;
	int			g;
	int			b;

	if (!input_ptr || !*input_ptr || !out_color)
		return (0);
	cursor = *input_ptr;
	// Serve per assicurarmi che dopo 255 ci sia subito "," e non altro altrimenti invalida subbito prima di proseguire
	/* R E G: devono essere seguiti da una virgola */
	if (!parse_RGB_component(&cursor, &r))
		return (0);
	if (!skip_comma(&cursor))
		return(0);
	if (!parse_RGB_component(&cursor, &g))
		return (0);
	if (!skip_comma(&cursor))
		return(0);
	if (!parse_RGB_component(&cursor, &b))
		return (0);
	out_color->r = (double)r / 255.0;
	out_color->g = (double)g / 255.0;
    out_color->b = (double)b / 255.0;
	*input_ptr = cursor;
	return (1);
}

/* Helper per la norm per parse_int controlla overflow */
static inline int check_value_max(int sign, long acc)
{
	if (sign == 1 && acc > (long)INT_MAX)
		return (0);
	if (sign == -1 && -acc < (long)INT_MIN)
			return (0);
	return (1);
}

/* Helper: legge segno, consuma le cifre con overflow-check, aggiorna cursor e out_value */
static int	parse_int_core(const char **cursor, int *out_value)
{
	long	acc;
	int		sign;

	acc = 0;
	sign = 1;
	if (!cursor || !*cursor || !out_value)
		return (0);
	if (**cursor == '+' || **cursor == '-')
	{
		if (**cursor == '-')
			sign = -1;
		(*cursor)++;
	}
	if (!ft_isdigit(**cursor))
		return (0);
	while (ft_isdigit(**cursor))
	{
		acc = acc * 10 + (**cursor - '0');
		if (!check_value_max(sign, acc))
			return (0);
		(*cursor)++;
	}
	*out_value = (int)(acc * sign);
	return (1);
}

/* Converte da stringa a intero ma restituisce anche il puntatorte alla stringa */
int	parse_int(const char **line_pointer, int *out_value)
{
	const char	*cursor;

	if (!line_pointer || !*line_pointer || !out_value)
		return (0);

	cursor = skip_spaces(*line_pointer);
	if (!parse_int_core(&cursor, out_value))
		return (0);

	*line_pointer = cursor;
	return (1);
}
