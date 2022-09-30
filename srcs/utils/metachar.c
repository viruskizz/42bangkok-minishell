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

int	is_opt(char *s)
{
	int	len;

	// if (!s)
	// 	return (0);
	// if (s[0] != '&' && s[0] != '|')
	// 	return (0);
	// len = ft_strlen(s);
	// if (s[0] == '&' && s[1] == '&' && len == 2)
	// 	return (1);
	// else if (s[0] == '|' && s[1] == '|' && len == 2)
	// 	return (1);
	// else if (s[0] == '|' && len == 1)
	// 	return (1);
	// else
	// 	return (-1);
	if (!s)
		return (0);
	if (*s != '&' && *s != '|')
		return (0);
	if (!ft_strcmp(s, "||"))
		return (1);
	else if (!ft_strcmp(s, "&&"))
		return (1);
	else if (!ft_strcmp(s, "|"))
		return (1);
	else
		return (-1);
}

int	parse_opt(char *opt)
{
	if (!ft_strcmp(opt, "||"))
		return (OPT_OR);
	else if (!ft_strcmp(opt, "&&"))
		return (OPT_AND);
	else if (!ft_strcmp(opt, "|"))
		return (OPT_PIPE);
	else
		return (OPT_NULL);
}
