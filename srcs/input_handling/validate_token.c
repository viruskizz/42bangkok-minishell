/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiva <tsomsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 16:54:51 by araiva            #+#    #+#             */
/*   Updated: 2022/09/28 16:54:52 by araiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_token(t_list **tokens)
{
	t_list	*lst;
	char	*s;
	char	*ns;

	lst = *tokens;
	ns = NULL;
	while (lst)
	{
		s = lst->content;
		if (lst->next)
			ns = lst->next->content;
		if ((is_opt(s) || is_redirect(s)) && !lst->next)
			return (0);
		else if (lst->next
			&& (is_opt(s) || is_redirect(s))
			&& (is_opt(ns) || is_redirect(ns)))
			return (0);
		lst = lst->next;
	}
	return (1);
}
