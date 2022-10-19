/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 23:26:59 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/16 15:56:36 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execution_command(t_shell *shell, t_execute *exe, t_cmd *cmds)
{	
	cmds = exe->cmds;
	execution_signal(shell, CHILD);
	if (exe->index == 0 && exe->xedni == 0)
	{
		if (redirect_infile(shell, exe->cmds) < 0)
			return (-1);
	}
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->terminal->shell);
	sigaction(SIGQUIT, &shell->sigquit, NULL);
	redirect_dup_start(shell, exe);
	if (string_compare(cmds->tokens[0], "cd", NO_LEN) == 1)
		shell->exstat = 0;
	else if (string_compare(cmds->tokens[0], "export", NO_LEN) == 1)
		shell->exstat = 0;
	else if (string_compare(cmds->tokens[0], "unset", NO_LEN) == 1)
		shell->exstat = 0;
	else if (string_compare(cmds->tokens[0], "env", NO_LEN) == 1)
		shell->exstat = execution_print_env(shell);
	else if (access(cmds->tokens[0], F_OK | R_OK | X_OK) == 0)
		shell->exstat = execution_token(shell, cmds->tokens[0], cmds->tokens);
	else
		execution_path_command(shell, cmds->tokens, 0);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->terminal->minishell);
	exit(shell->exstat);
}

int	execution_path_command(t_shell *shell, char **command, int index)
{
	char	*path;
	char	**env_path;

	env_path = ft_split_mode(environment_getenv("PATH", shell), ':', BOUND);
	while (env_path[index] != NULL)
	{
		path = ft_midjoin(env_path[index++], command[0], '/');
		if (access(path, F_OK | R_OK | X_OK) == 0)
		{
			shell->exstat = execution_token(shell, path, command);
			if (shell->exstat == -1 && free_db_ptr(NULL, env_path, path))
				return (-1);
			break ;
		}
		else if (env_path[index + 1] == NULL)
		{
			printf("minishell: command not found: %s\n", command[0]);
			shell->exstat = 127;
			break ;
		}
		free(path);
	}
	free_db_ptr(NULL, env_path, path);
	return (0);
}

int	execution_token(t_shell *shell, char *path, char **command)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	else if (pid == 0)
	{
		execve(path, command, NULL);
	}
	else if (pid > 0)
	{
		waitpid(pid, &shell->exstat, 0);
		if (shell->exstat == 256)
			return (1);
	}
	return (0);
}

int	redirect_dup_input(int *fd)
{
	int		infile;

	infile = open(IN_FILE, O_RDONLY);
	if (infile < 0)
	{
		close(fd[0]);
		return (0);
	}
	dup2(infile, 0);
	close(infile);
	close(fd[0]);
	return (0);
}

int	redirect_dup_start(t_shell *shell, t_execute *exe)
{
	int		file;
	t_cmd	*cmd;

	file = 0;
	cmd = (t_cmd *)shell->cmds->content;
	redirect_dup_input(exe->fd);
	if (cmd->fg == NULL && cmd->fgg == NULL && cmd->opt == OPT_PIPE)
		dup2(exe->fd[1], 1);
	else if (cmd->fg != NULL && cmd->fg[exe->index] != NULL)
		file = open(cmd->fg[exe->index++], O_RDWR | O_CREAT, 0644);
	else if ((cmd->fgg != NULL && cmd->fgg[exe->xedni] != NULL))
		file = open(cmd->fgg[exe->xedni++], O_RDWR | O_CREAT | O_APPEND, 0644);
	if (file < -1)
		return (-1);
	else if (file > 2)
	{
		dup2(file, 1);
		close(file);
	}
	close(exe->fd[1]);
	return (0);
}
