/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 15:06:18 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/21 15:10:30 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	do_export_env(t_shell *shell, char *cmd);
static char	*parsing_export_cmd(char **cmds, int *index, int quote, int qquote);

int	execution_export_env(t_shell *shell, char **cmds, int index)
{
	char	*cmd;
	int		xedni;

	while (cmds != NULL && cmds[++index] != NULL)
	{
		xedni = 0;
		while (1 > 0)
		{
			if (cmds[index][xedni] == '\0')
			{
				cmd = ft_strdup(cmds[index]);
				break ;
			}
			if (cmds[index][xedni] == '\'' || cmds[index][xedni] == '\"')
			{
				cmd = parsing_export_cmd(cmds, &index, 0, 0);
				break ;
			}
			xedni++;
		}
		if (do_export_env(shell, cmd) == -1)
			return (-1);
		free_db_ptr(NULL, NULL, cmd);
	}
	return (0);
}

static char	*parsing_export_cmd(char **cmds, int *index, int quote, int qquote)
{
	int		xedni;
	char	*cmd;

	cmd = NULL;
	while (cmds[(*index)] != NULL)
	{
		xedni = -1;
		while (cmds[(*index)][++xedni] != '\0')
		{
			if (cmds[(*index)][xedni] == '\'')
				quote++;
			else if (cmds[(*index)][xedni] == '\"')
				qquote++;
		}
		if (quote % 2 == 0 && qquote % 2 == 0)
		{
			if (cmd == NULL)
				return (strtranfer_no_quote(cmd, cmds[(*index)]));
			else
				return (midjoin_free(cmd, cmds[(*index)], ' '));
		}
		else
			cmd = midjoin_free(cmd, cmds[(*index)++], ' ');
	}
	return (cmd);
}

static int	do_export_env(t_shell *shell, char *cmd)
{
	char	*var_name;
	char	*var_value;

	var_name = environment_get_name(cmd);
	if (var_name == NULL)
		return (-1);
	if (environment_check_name(var_name, cmd, shell) != 0)
	{
		free(var_name);
		return (1);
	}
	var_value = environment_get_value(cmd);
	if (var_value == NULL)
	{
		free(var_name);
		return (1);
	}
	environment_export_env(shell, var_name, var_value, cmd);
	free(var_value);
	free(var_name);
	return (0);
}
