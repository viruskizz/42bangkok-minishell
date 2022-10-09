/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 18:30:21 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/10 00:06:05 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execution_unset_env(t_env **env, char **variable_name, int index)
{
	t_env	*current;
	t_env	*check;

	while (variable_name[++index] != NULL)
	{
		current = *env;
		check = *env;
		if (string_compare((*env)->name, variable_name[index], NO_LEN) == 1)
		{
			*env = (*env)->next;
			environment_delete(current);
		}
		while (current != NULL)
		{
			check = check->next;
			if (check != NULL && string_compare(check->name, variable_name[index], NO_LEN) == 1)
			{
				current->next = check->next;
				environment_delete(check);
			}
			current = current->next;
		}
	}
	return (0);
}

void	environment_delete(t_env *env)
{
	free(env->name);
	free(env->value);
	free(env);
}

char	*environment_getenv(char *variable_name, t_shell *shell)
{
	while (shell->env != NULL)
	{
		if (string_compare(variable_name, shell->env->name, NO_LEN) == 1)
		{
			return (ft_strdup(shell->env->value));
		}
		shell->env = shell->env->next;
	}
	return (0);
}
