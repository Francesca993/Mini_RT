/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 12:27:30 by fmontini          #+#    #+#             */
/*   Updated: 2024/12/26 12:27:32 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*final;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	final = ft_lstlast(*lst);
	final->next = new;
}
