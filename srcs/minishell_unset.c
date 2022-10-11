/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 18:30:21 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/11 23:11:29 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* unset ther environment in t_env can do more than one time*/
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

/* helper of environment_upset_env */
void	environment_delete(t_env *env)
{
	free(env->name);
	free(env->value);
	free(env);
}

/* function get the value of t_env */
/* funntion working same with getenv() */
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
