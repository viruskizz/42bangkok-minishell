/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_util2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 17:25:00 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/14 23:30:34 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double_pointer(char **str1, char **str2, void *str3)
{
	int	index;

	index = 0;
	if (str1 != NULL)
	{
		while (str1[index] != NULL)
			free(str1[index++]);
		free(str1);
		str1 = NULL;
	}
	index = 0;
	if (str2 != NULL)
	{
		while (str2[index] != NULL)
			free(str2[index++]);
		free(str2);
		str2 = NULL;
	}
	if (str3 != NULL)
	{
		free(str3);
		str3 = NULL;
	}
}

/* helper of environment_upset_env */
void	environment_delete(t_env *env)
{
	free(env->name);
	free(env->value);
	free(env);
}

void	execution_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		exit(1);
	}
	else if (signum == SIGQUIT)
	{
		printf("Quit: 3\n");
		exit(131);
	}
}

char	**doublepointer_join_copy(char **result, char **strs, int *index)
{
	int	len;
	int	xedni;

	len = ft_lencount(NULL, strs, STRS);
	result = (char **)ft_calloc(sizeof(char *), len + 2);
	if (result == NULL)
		return (0);
	while (strs != NULL && strs[*index] != NULL)
	{
		len = ft_lencount(strs[*index], NULL, STR);
		result[*index] = (char *)ft_calloc(sizeof(char), len + 1);
		if (result == NULL)
		{
			free_double_pointer(result, NULL, NULL);
			return (0);
		}
		xedni = 0;
		while (strs[*index][xedni] != '\0')
		{
			result[*index][xedni] = strs[*index][xedni];
			xedni++;
		}
		(*index)++;
	}
	return (result);
}

/* function join a string into a double pointer */
char	**doublepointer_join(char **strs, char *str)
{
	char	**result;
	int		index;
	int		xedni;

	index = 0;
	result = doublepointer_join_copy(result, strs, &index);
	if (result == NULL)
		return (0);
	xedni = 0;
	result[index] = ft_calloc(sizeof(char), ft_lencount(str, NULL, STR) + 1);
	if (result == NULL)
	{
		free_double_pointer(result, NULL, NULL);
		return (0);
	}
	while (str != NULL && str[xedni] != '\0')
	{
		result[index][xedni] = str[xedni];
		xedni++;
	}
	result[++index] = NULL;
	free_double_pointer(strs, NULL, NULL);
	return (result);
}
