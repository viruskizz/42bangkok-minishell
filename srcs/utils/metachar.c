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

int	is_closing_quote(char c)
{
	if (c == '"' || c == '\'')
		return (1);
	else
		return (0);
}

int	is_opt(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (len > 2 || len == 0)
		return (OPT_NULL);
	if (ft_strcmp(str, "&&"))
		return (OPT_AND);
	if (ft_strcmp(str, "||"))
		return (OPT_OR);
	if (ft_strcmp(str, "|"))
		return (OPT_PIPE);
	return (OPT_NULL);
}

int	is_spt(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	else
		return (0);
}
