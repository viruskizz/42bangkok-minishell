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

// int	is_opt(char *s)
// {
// 	int	len;

// 	if (!s)
// 		return (0);
// 	if (*s != '&' && *s != '|')
// 		return (0);
// 	if (!ft_strcmp(s, "||"))
// 		return (2);
// 	else if (!ft_strcmp(s, "&&"))
// 		return (2);
// 	else if (!ft_strcmp(s, "|"))
// 		return (1);
// 	else
// 		return (-1);
// }

int	is_opt(char *s)
{
	if (!s)
		return (0);
	if (*s != '&' && *s != '|')
		return (0);
	if (!ft_strncmp(s, "||", 2))
		return (2);
	else if (!ft_strncmp(s, "&&", 2))
		return (2);
	else if (!ft_strncmp(s, "|", 1))
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

char	*str_opt(int opt)
{
	if (opt == OPT_OR)
		return ("||");
	else if (opt == OPT_AND)
		return ("&&");
	else if (opt == OPT_PIPE)
		return ("|");
	else
		return (NULL);
}

int	is_redirect(char *str)
{
	if (!str)
		return (0);
	if (*str != '<' && *str != '>')
		return (0);
	if (!ft_strncmp(str, "<<", 2))
		return (2);
	else if (!ft_strncmp(str, ">>", 2))
		return (2);
	else if (!ft_strncmp(str, "<", 1))
		return (1);
	else if (!ft_strncmp(str, ">", 1))
		return (1);
	else
		return (-1);
}
