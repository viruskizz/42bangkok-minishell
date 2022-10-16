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

int	free_db_ptr(char **str1, char **str2, void *str3)
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
	return (1);
}

/* helper of environment_upset_env */
void	environment_delete(t_env *env)
{
	free(env->name);
	free(env->value);
	free(env);
}
