/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 15:57:30 by fmontini          #+#    #+#             */
/*   Updated: 2024/12/14 13:04:27 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	uc;

	uc = (unsigned char )c;
	while (*s)
	{
		if (*s == uc)
		{
			return ((char *)s);
		}
		s++;
	}
	if (uc == '\0')
	{
		return ((char *)s);
	}
	return (NULL);
}
