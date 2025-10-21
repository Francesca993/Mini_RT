/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 11:56:38 by fmontini          #+#    #+#             */
/*   Updated: 2024/12/26 16:14:38 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(const char *s, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
		{
			i++;
		}
		if (s[i] != '\0')
		{
			count++;
			while (s[i] != '\0' && s[i] != c)
				i++;
		}
	}
	return (count);
}

static int	ft_wlen(const char *s, char c)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != c)
	{
		len ++;
	}
	return (len);
}

static char	**ft_matrix(const char *s, char c, char **result)
{
	int		i;
	int		word_len;

	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			word_len = ft_wlen(s, c);
			result[i] = ft_substr(s, 0, word_len);
			if (!result[i])
			{
				while (i > 0)
					free(result[--i]);
				free(result);
				return (NULL);
			}
			i++;
			s += word_len;
		}
		else
			s++;
	}
	result[i] = NULL;
	return (result);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		count;

	if (!s)
		return (NULL);
	count = ft_count(s, c);
	result = (char **)malloc((count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	result = ft_matrix(s, c, result);
	return (result);
}
