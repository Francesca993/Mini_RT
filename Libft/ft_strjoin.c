/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 12:02:17 by fmontini          #+#    #+#             */
/*   Updated: 2024/12/26 12:06:42 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	size_t	tot_len;
	char	*n_str;

	i = 0;
	j = 0;
	tot_len = ft_strlen(s1) + ft_strlen(s2);
	n_str = (char *)malloc(tot_len + 1);
	if (!s1 || !s2)
		return (NULL);
	if (!n_str)
		return (NULL);
	while (s1[i])
	{
		n_str[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		n_str[i + j] = s2[j];
		j++;
	}
	n_str[i + j] = 0;
	return (n_str);
}
