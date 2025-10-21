/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 12:24:11 by fmontini          #+#    #+#             */
/*   Updated: 2024/12/26 12:24:40 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	s_len;
	char	*dup;

	s_len = ft_strlen(s1)+ 1;
	dup = (char *)malloc(s_len);
	if (dup == NULL)
		return (NULL);
	ft_strlcpy(dup, s1, s_len);
	return (dup);
}
