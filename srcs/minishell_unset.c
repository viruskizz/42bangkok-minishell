/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 18:30:21 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/06 16:25:12 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execution_unset_env(t_env **env, char **variable_name)
{
	t_env	*current;
	t_env	*check;
	int		index;

	index = 1;
	current = *env;
	check = *env;
	while (variable_name[index] != NULL)
	{
		if (string_compare((*env)->name, variable_name[index], NO_LEN) == 1)
		{
			*env = (*env)->next;
			environment_delete(current);
			// free(current->name);
			// free(current->value);
			// free(current);
			//return (0);
		}
		while (current != NULL)
		{
			check = check->next;
			if (check != NULL && string_compare(check->name, variable_name[index], NO_LEN) == 1)
			{
				current->next = check->next;
				environment_delete(check);
				// free(check->name);
				// free(check->value);
				// free(check);
				//return (0);
			}
			current = current->next;
		}
		index++;
	}
	return (0);
}

void	environment_delete(t_env *env)
{
	free(env->name);
	free(env->value);
	free(env);
}
