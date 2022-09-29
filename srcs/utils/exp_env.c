/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiva <tsomsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 16:51:30 by araiva            #+#    #+#             */
/*   Updated: 2022/09/28 16:51:31 by araiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*var_val(char *str);
static int	var_len(char *s);

int	exp_env(char *token, char **str)
{
	int		wlen;
	char	*new;
	char	*var;

	if (*token != '$')
		return (0);
	if (*(token + 1) == '?')
	{
		wlen = 2;
		var = ft_strdup("0");
	}
	else
	{
		wlen = var_len(token);
		var = var_val(token);
	}
	*str = my_strcat(*str, var);
	return (wlen);
}

int	exp_env_hom(char *token, char **str)
{
	int		wlen;
	char	*new;
	char	*var;

	if (*token != '~')
		return (0);
	wlen = 2;
	var = var_val("HOME");
	*str = my_strcat(*str, var);
	return (wlen);
}

/*
 * word length is not including space char
 */
static int	var_len(char *s)
{
	int		i;
	char	*pattern;

	i = 0;
	pattern = " $*\"\t\0";
	if (*s != '$')
		return (i);
	while (ft_isalnum(s[++i]))
		;
	return (i);
}

static char	*var_val(char *str)
{
	int		vlen;
	char	*var;
	char	*val;

	vlen = var_len(str);
	var = ft_calloc(vlen + 1, sizeof(char));
	ft_strlcpy(var, str, vlen + 1);
	if (getenv(var + 1))
		val = ft_strdup(getenv(var + 1));
	else
		val = ft_calloc(1, sizeof(char));
	free(var);
	return (val);
}
