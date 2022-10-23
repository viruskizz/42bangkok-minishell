/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_str.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 16:56:17 by araiva            #+#    #+#             */
/*   Updated: 2022/10/24 01:46:26 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exp_str(char *token, char **str)
{
	int		wlen;
	char	*s;

	wlen = 0;
	while (!ft_strchr("$'\"\0", token[wlen]))
		wlen++;
	s = ft_calloc(wlen + 1, sizeof(char));
	ft_strlcpy(s, token, wlen + 1);
	*str = my_strcat(*str, s);
	return (wlen);
}

int	exp_sq_str(char *token, char **str)
{
	int		wlen;
	char	*s;

	if (*token != '\'')
		return (0);
	wlen = 1;
	while (token[wlen] != '\'')
		wlen++;
	s = ft_calloc(wlen, sizeof(char));
	ft_strlcpy(s, token + 1, wlen);
	*str = my_strcat(*str, s);
	return (wlen + 1);
}

int	exp_dq_str(char *token, char **str)
{
	int		wlen;
	char	*s;

	wlen = 0;
	if (*token == '"')
		token++;
	while (!ft_strchr("$\"\0", token[wlen]))
		wlen++;
	s = ft_calloc(wlen + 1, sizeof(char));
	ft_strlcpy(s, token, wlen + 1);
	*str = my_strcat(*str, s);
	return (wlen);
}
