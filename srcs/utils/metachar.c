/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metachar.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiva <tsomsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 19:04:47 by araiva            #+#    #+#             */
/*   Updated: 2022/09/25 19:04:49 by araiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief 
 * 
 * @param lst 
 * @return int 0 is normal string, 1 is opt, -1 is invalid
 */
int	validate_opt(t_list *lst)
{
	int	is_t;

	is_t = is_opt((char *) lst->content);
	if (is_t == 0)
		return (0);
	if (is_t < 0)
		return (-1);
	if (is_t && !lst->next)
		return (-1);
	if (is_t && is_opt((char *) lst->next->content))
		return (-1);
	return (1);
}

int	is_opt(char *s)
{
	int	len;

	if (!s)
		return (0);
	if (s[0] != '&' && s[0] != '|')
		return (0);
	len = ft_strlen(s);
	if (s[0] == '&' && s[1] == '&' && len == 2)
		return (1);
	else if (s[0] == '|' && s[1] == '|' && len == 2)
		return (1);
	else if (s[0] == '|' && len == 1)
		return (1);
	else
		return (-1);
}
