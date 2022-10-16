/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 20:38:04 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/16 15:56:15 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_single_cmd(t_shell *shell, t_execute *exe);

void	execution_increasement(t_shell *shell, t_execute *exe)
{
	t_cmd	*cmd;

	cmd = ((t_cmd *)shell->cmds->content);
	if (cmd->fg != NULL && cmd->fg[exe->index] != NULL)
	{
		exe->index++;
	}
	else if ((cmd->fgg != NULL && cmd->fgg[exe->xedni] != NULL))
	{
		exe->xedni++;
	}
	else if (cmd->fg != NULL && cmd->fgg != NULL)
	{
		exe->index++;
	}
}

int	execution_waitpid(t_shell *shell, t_execute *exe)
{
	t_cmd	*cmds;
	int		exit_status;

	cmds = exe->cmds;
	waitpid(exe->pid, &exit_status, 0);
	shell->exstat = WEXITSTATUS(exit_status);
	if (string_compare(cmds->tokens[0], "cd", NO_LEN) == 1)
		execution_change_directory(shell, cmds->tokens);
	else if (string_compare(cmds->tokens[0], "export", NO_LEN) == 1)
	{
		if (execution_export_env(shell, cmds->tokens, 0) == -1)
			return (-42);
	}
	else if (string_compare(cmds->tokens[0], "unset", NO_LEN) == 1)
		shell->exstat = execution_unset_env(&shell->env, cmds->tokens, 0);
	if (shell->cmds->next != NULL && ((t_cmd *)shell->cmds->content)->opt == 1)
		dup2(exe->fd[0], 0);
	else
		dup2(shell->sinput, 0);
	close(exe->fd[0]);
	close(exe->fd[1]);
	execution_increasement(shell, exe);
	if (exe->index + exe->xedni <= exe->files)
		return (1);
	return (0);
}

void	executeion_inite(t_shell *shell, t_execute *exe)
{
	exe->index = 0;
	exe->xedni = 0;
	exe->execute = 0;
	exe->cmds = (t_cmd *)shell->cmds->content;
	exe->files = ft_lencount(NULL, exe->cmds->fg, STRS);
	exe->files += ft_lencount(NULL, exe->cmds->fgg, STRS);
}

int	cmd_execution(t_shell *shell)
{
	t_execute	exe;
	t_list		*cmd;

	exe.execute = 0;
	cmd = shell->cmds;
	execution_signal(shell, PARENT_I);
	while (shell->cmds != NULL && exe.execute != -42)
	{
		executeion_inite(shell, &exe);
		if (exec_single_cmd(shell, &exe) < 0)
			return (-1);
		if (stat(IN_FILE, &exe.info) == 0)
			unlink(IN_FILE);
		if ((exe.cmds->opt == OPT_AND && shell->exstat != 0)
			|| (exe.cmds->opt == OPT_OR && shell->exstat == 0))
			break ;
		shell->cmds = shell->cmds->next;
	}
	shell->cmds = cmd;
	execution_signal(shell, PARENT_O);
	return (0);
}

static int	exec_single_cmd(t_shell *shell, t_execute *exe)
{
	while (exe->index + exe->xedni < exe->files || exe->execute == 0)
	{
		if (pipe(exe->fd) < 0)
			return (-1);
		exe->pid = fork();
		if (exe->pid < 0)
			return (-1);
		if (exe->pid == 0)
			execution_command(shell, exe, NULL);
		else if (exe->pid > 0)
			exe->execute = execution_waitpid(shell, exe);
	}
	return (1);
}
