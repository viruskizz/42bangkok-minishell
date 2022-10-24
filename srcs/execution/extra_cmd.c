/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 23:33:02 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/23 23:51:35 by sharnvon         ###   ########.fr       */
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
		print_error(directory, NULL, CD_NODIR);
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
		print_error(command[1], NULL, CD_PWD);
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
		ft_putstr_fd("minishell: cd: too many agurments\n", 2);
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

int	execution_export_env(t_shell *shell, char **cmds, int index)
{
	char	*var_name;
	char	*var_value;

	shell->exstat = 0;
	while (cmds != NULL && cmds[++index] != NULL)
	{
		var_name = environment_get_name(shell, cmds[index]);
		if (var_name == NULL)
			continue ;
		if (environment_check_name(var_name, cmds[index], shell) != 0)
		{
			free(var_name);
			continue ;
		}
		var_value = environment_get_value(cmds[index]);
		if (var_value == NULL)
		{
			free(var_name);
			continue ;
		}
		environment_export_env(shell, var_name, var_value, cmds[index]);
		free(var_value);
		free(var_name);
	}
	return (0);
}
