/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc_convert.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 01:30:39 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/13 18:12:55 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*convert_env_find(t_shell *shell, char *str, int *i)
{
	char	*env_name;
	t_env	*env;
	int		len;

	env = shell->env;
	len = 0;
	while (ft_isalnum(str[(*i + 1) + len]) == 1 || str[(*i + 1) + len] == '_')
		len++;
	env_name = (char *)ft_calloc(sizeof(char), len + 1);
	if (env_name == NULL)
		return (0);
	env_name = string_tranfer(env_name, &str[(*i) + 1], len);
	while (env != NULL)
	{
		if (string_compare(env_name, env->name, NO_LEN) == 1)
		{
			free(env_name);
			return (env);
		}
		env = env->next;
	}
	while ((str[*i] < '\t' || str[*i] > '\r') && str[*i] != ' ')
		(*i)++;
	free(env_name);
	return (NULL);
}

int	convert_env_tranfer(t_env *env, char *buff, int *index, int *xedni)
{
	int	count;

	count = 0;
	if (env == NULL)
		return (-1);
	while (env->value[count] != '\0')
	{
		buff[(*xedni)++] = env->value[count++];
	}
	(*index) = (*index) + ft_lencount(env->name, NULL, STR) + 1;
	return (0);
}

int	convert_string_tranfer(char *str, char *buff, int *index, int *xedni)
{
	int	count;

	count = 0;
	if (str == NULL)
		return (-1);
	while (str[count] != '\0')
	{
		buff[(*xedni)++] = str[count++];
	}
	(*index) = (*index) + 2;
	return (0);
}

char	*heredoc_convert_env(t_shell *shell, char *buff, int index, int xedni)
{
	t_env	*env;
	char	*str;
	char	*exit_stat;

	exit_stat = NULL;
	env = NULL;
	str = ft_strdup(buff);
	ft_bzero((void *)buff, 10000);
	while (str[index] != '\0')
	{
		if (str[index] == '$')
		{
			if (str[index + 1] == '?')
				exit_stat = ft_itoa(shell->exstat);
			else
				env = convert_env_find(shell, str, &index);
			convert_env_tranfer(env, buff, &index, &xedni);
			convert_string_tranfer(exit_stat, buff, &index, &xedni);
		}
		else
			buff[xedni++] = str[index++];
	}
	buff[xedni] = '\0';
	free_db_ptr(NULL, NULL, str);
	return (buff);
}
