/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 04:07:40 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/09 23:59:26 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* function find environment variable name malloc and return */
/* mode:ENVI:0 use in make env | mode:COMM:1 use in export */
char	*environment_get_name(char *command)
{
	char	*variable_name;
	int		index;

	index = 0;
	while (command[index] != '\0' && command[index] != '=')
		index++;
	variable_name = (char *)ft_calloc(sizeof(char), (index + 1));
	if (variable_name == NULL)
		return (0);
	index = 0;
	while (command[index] != '\0' && command[index] != '=')
	{
		variable_name[index] = command[index];
		index++;
	}
	variable_name[index] = '\0';
	return (variable_name);
}

/* -1 is false | 0 is true */
int	environment_check_name(char *variable_name)
{
	int	index;

	index = 0;
	/* check upper letter & digit & _ */
	if (ft_isalpha(variable_name[0]) == 0 && variable_name[0] != '_')
	{
		printf("export: not an identifier: %s\n", variable_name);
		return (-1);
	}
	/* inside variable name must be alphabet or number */
	while (variable_name[index] != '\0')
	{
		if (ft_isalnum(variable_name[index++]) == 0)
		{
			printf("export: not an identifier: %s\n", variable_name);
			return (-1);
		}
	}
	return (0);
}

/* function check start point and  pair of qoutes in environment value */
/* mode:START return the start point | mode:QUOTES return amount of quote*/
/* return 0 if no environment value && return -1 if quote or qquote % 2 != 0 */
int	environment_check_value(char *command, int quote, int qquote, int mode)
{
	int		index;
	int		start;

	index = 0;
	while (command[index] != '=' && command[index] != '\0')
		index++;
	// if (command[index] == '\0') // * may be not need //
	// 	return (0);
	start = index + 1;
	index = 0;
	while (command[start + index] != '\0')
	{
		if (command[start + index] == '\"')
			qquote++;
		if (command[start + index++] == '\'')
			quote++;
	}
	if (quote % 2 != 0 || qquote % 2 != 0)
	{
		printf("minishell: invalid quotes\n");
		return (-1);
	}
	if (mode == START)
		return (start);
	if (mode == INDEX)
		return (index);
	return (qquote + quote);
}

/* if environment_value doesn't existe return "" */
/* if environment_value doesn't got pair of quote return NULL */
char	*environment_get_value(char *command)
{
	char	*variable_value;
	int		index;
	int		start;
	int		quote;
	int		xedni;

	start = environment_check_value(command, 0, 0, START);
	if (start == -1)
		return (0);
	quote = environment_check_value(command, 0, 0, QUOTE);
	index = environment_check_value(command, 0, 0, INDEX);
	variable_value = (char *)ft_calloc(sizeof(char), ((index - quote) + 1));
	if (variable_value == NULL)
		return (0);
	index = 0;
	xedni = 0;
	while (command[start + index] != '\0')
	{
		if (command[start + index] != '\"')
			variable_value[xedni++] = command[start + index];
		index++;
	}
	variable_value[xedni] = '\0';
	return (variable_value);
}

int	environment_export_env(t_shell *shell, char *name, char *value, char *cmd)
{
	t_env	*count;

	count = shell->env;
	while (count != NULL)
	{
		if (string_compare(count->name, name, NO_LEN) == 1)
		{
			free(count->value);
			count->value = ft_strdup(value);
			break ;
		}
		count = count->next;
	}
	if (count == NULL)
		environment_add_back(&shell->env, environment_new(cmd));
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
		if (environment_check_name(var_name) != 0)
		{
			free(var_name);
			shell->exstat = 1;
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
