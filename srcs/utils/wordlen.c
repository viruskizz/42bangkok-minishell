/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordlen.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiva <tsomsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 08:19:15 by araiva            #+#    #+#             */
/*   Updated: 2022/10/22 08:19:16 by araiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	qlen(char *str);

int	wordlen(char *str)
{
	int	i;

	i = 0;
	if (is_redirect(str))
		return (is_redirect(str));
	else if (is_opt(str))
		return (is_opt(str));
	while (str[i] && !ft_strchr(" \n\t<>|&", str[i]))
	{
		if (ft_strchr(QUOTES, str[i]))
			i += qlen(&str[i]);
		else
			i++;
	}
	return (i);
}

static int	qlen(char *str)
{
	int		i;

	if (!ft_strchr(QUOTES, *str))
		return (0);
	i = 1;
	while (str[i] && str[i] != str[0])
		i++;
	return (++i);
}
