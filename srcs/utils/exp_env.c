/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 16:51:30 by araiva            #+#    #+#             */
/*   Updated: 2022/10/13 23:13:48 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*var_val(char *str, t_shell *shell);
static int	var_len(char *s);

int	exp_env(char *token, char **str, t_shell *shell)
{
	int		wlen;
	char	*var;

	if (*token != '$')
		return (0);
	if (*(token + 1) == '?')
	{
		var = ft_itoa(shell->exstat);
		wlen = 2;
	}
	else
	{
		wlen = var_len(token);
		var = var_val(token, shell);
	}
	*str = my_strcat(*str, var);
	return (wlen);
}

int	exp_env_hom(char *token, char **str, t_shell *shell)
{
	int		wlen;
	char	*var;

	if (*token != '~')
		return (0);
	if (ft_strcmp(token + 1, environment_getenv("USER", shell)))
		wlen = 1;
	else
		wlen = ft_strlen(token);
	var = environment_getenv("HOME", shell);
	*str = my_strcat(*str, ft_strdup(var));
	*str = my_strcat(*str, ft_strdup("/"));
	return (wlen);
}

/*
 * word length is not including space char
 */
static int	var_len(char *s)
{
	int		i;

	i = 0;
	if (*s != '$')
		return (i);
	while (ft_isalnum(s[++i]))
		;
	return (i);
}

static char	*var_val(char *str, t_shell *shell)
{
	int		vlen;
	char	*var;
	char	*val;

	vlen = var_len(str);
	var = ft_calloc(vlen + 1, sizeof(char));
	ft_strlcpy(var, str, vlen + 1);
	if (environment_getenv(var + 1, shell))
		val = ft_strdup(environment_getenv(var + 1, shell));
	else
		val = ft_calloc(1, sizeof(char));
	free(var);
	return (val);
}
