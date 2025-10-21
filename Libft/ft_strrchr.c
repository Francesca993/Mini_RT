/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:24:37 by fmontini          #+#    #+#             */
/*   Updated: 2024/12/26 13:45:39 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t			i;
	unsigned char	c_cast;

	i = ft_strlen(s);
	c_cast = (unsigned char)c;
	if (c_cast == '\0')
	{
		return ((char *)s + i);
	}
	while (i > 0)
	{
		i--;
		if ((char)s[i] == c_cast)
		{
			return ((char *)s + i);
		}
	}
	return (NULL);
}
