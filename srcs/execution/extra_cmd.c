/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 23:33:02 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/14 23:29:02 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_directory(t_shell *shell, char *directory)
{
	char	*path_env;
	char	*command;

	path_env = (char *)ft_calloc(sizeof(char), 1000);
	if (path_env == NULL)
	{
		perror("minishell");
		return (-1);
	}
	getcwd(path_env, 999);
	command = ft_midjoin("OLDPWD", path_env, '=');
	if (chdir(directory) != 0)
	{
		printf("minishell: cd: no such file or directory: %s\n", directory);
		shell->exstat = 1;
	}
	environment_export_env(shell, "OLDPWD", path_env, command);
	free(command);
	getcwd(path_env, 999);
	command = ft_midjoin("PWD", path_env, '=');
	environment_export_env(shell, "PWD", path_env, command);
	free(command);
	free(path_env);
	return (0);
}

int	execution_change_directory(t_shell *shell, char **command)
{
	char	*user;

	user = ft_midjoin("~", environment_getenv("USER", shell), '\0');
	if (ft_lencount(NULL, command, STRS) == 3)
	{
		printf("minishell: cd: string not in pwd: %s\n", command[1]);
		shell->exstat = 1;
	}
	else if (ft_lencount(NULL, command, STRS) == 1
		|| string_compare(command[1], "~", NO_LEN) == 1
		|| string_compare(command[1], user, NO_LEN) == 1)
		change_directory(shell, environment_getenv("HOME", shell));
	else if (string_compare(command[1], "-", NO_LEN) == 1)
		change_directory(shell, environment_getenv("OLDPWD", shell));
	else if (ft_lencount(NULL, command, STRS) == 2)
		change_directory(shell, command[1]);
	else
	{
		printf("minishell: cd: too many agurments\n");
		shell->exstat = 1;
	}
	free(user);
	return (0);
}

int	execution_print_env(t_shell *shell)
{
	t_env	*environment;

	environment = shell->env;
	if (environment == NULL)
		return (1);
	while (environment != NULL)
	{
		printf("%s=%s\n", environment->name, environment->value);
		environment = environment->next;
	}
	return (0);
}

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
			if (check != NULL && string_compare(check->name,
					variable_name[index], NO_LEN) == 1)
			{
				current->next = check->next;
				environment_delete(check);
			}
			current = current->next;
		}
	}
	return (0);
}

int	execution_export_env(t_shell *shell, char **cmds, int index)
{
	char	*var_name;
	char	*var_value;

	shell->exstat = 0;
	while (cmds != NULL && cmds[++index] != NULL)
	{
		var_name = environment_get_name(cmds[index]);
		if (var_name == NULL)
			return (-1);
		if (environment_check_name(var_name, cmds[index], shell) != 0)
		{
			free(var_name);
			continue ;
		}
		var_value = environment_get_value(cmds[index]);
		if (var_value == NULL)
		{
			free(var_name);
			return (-1);
		}
		environment_export_env(shell, var_name, var_value, cmds[index]);
		free(var_value);
		free(var_name);
	}
	return (0);
}
