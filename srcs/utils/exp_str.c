/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_str.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiva <tsomsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 16:56:17 by araiva            #+#    #+#             */
/*   Updated: 2022/09/28 16:56:18 by araiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	word_len(char *s);

int	exp_str(char *token, char **str)
{
	int		wlen;
	char	*s;

	wlen = word_len(token);
	s = ft_calloc(wlen + 1, sizeof(char));
	ft_strlcpy(s, token, wlen + 1);
	*str = my_strcat(*str, s);
	return (wlen);
}

/*
 * word length is including space char
 */
static int	word_len(char *s)
{
	int		i;
	char	*pattern;

	i = 0;
	pattern = "$\0";
	while (!ft_strchr(pattern, s[++i]))
		;
	return (i);
}
