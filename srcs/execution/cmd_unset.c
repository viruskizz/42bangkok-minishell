/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 23:49:19 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/23 23:56:02 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	unset_validation(t_shell *shell, char *variable_name);
static void	environment_delete(t_env *env);

/* unset ther environment in t_env can do more than one time*/
void	execution_unset_env(t_shell *shell, t_env **env, char **name, int index)
{
	t_env	*current;
	t_env	*check;

	while (name[++index] != NULL)
	{
		current = *env;
		check = *env;
		if (unset_validation(shell, name[index]) == -1)
			continue ;
		if (string_compare((*env)->name, name[index], NO_LEN) == 1)
		{
			*env = (*env)->next;
			environment_delete(current);
		}
		while (current != NULL)
		{
			check = check->next;
			if (check && string_compare(check->name, name[index], NO_LEN) == 1)
			{
				current->next = check->next;
				environment_delete(check);
			}
			current = current->next;
		}
	}
}

static int	unset_validation(t_shell *shell, char *variable_name)
{
	if (ft_isalpha(variable_name[0]) == 0 && variable_name[0] != '_')
	{
		print_error(variable_name, "unset", ENV_NAME);
		shell->exstat = 1;
		return (-1);
	}
	return (0);
}

/* helper of environment_upset_env */
static void	environment_delete(t_env *env)
{
	free(env->name);
	free(env->value);
	free(env);
}
