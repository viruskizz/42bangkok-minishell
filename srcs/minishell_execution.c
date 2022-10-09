/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 20:38:04 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/10 00:00:00 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execution_path_command(t_shell *shell, char **command)
{
	int		index;
	char	*path;
	char	**env_path;

// * check not real path should path in shell->env
	env_path = ft_split_mode(getenv("PATH"), ':', BOUND);  //? FREE ?? //

	index = 0;
	while (env_path[index] != NULL)
	{
		path = ft_midjoin(env_path[index++], command[0], '/');
		//* command is exist and can execute */
		if (access(path, F_OK | R_OK | X_OK) == 0)
		{
			execution_token(shell, path, command);
			if (shell->exstat == -1)
			{
				free_double_pointer(command, env_path, path);
				return (-1);
			}
			break ;
		}
		//* command is not exist and can't execute */
		else if (env_path[index + 1] == NULL)
		{
			printf("minishell: command not found: %s\n", command[0]);
			shell->exstat = 127;		
		}
	}
	free_double_pointer(command, env_path, path);
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
	if (cmd->fg == NULL && cmd->fgg == NULL && cmd->opt != 0)
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

void	signal_defualt(void)
{
	struct sigaction	signal;

	sigemptyset(&signal.sa_mask);
	signal.sa_flags = 0;
	signal.sa_handler = SIG_DFL;
	sigaction(SIGINT, &signal, NULL);
	sigaction(SIGQUIT, &signal, NULL);
}

int	execution_change_directory(t_shell *shell, char **command)
{
	char	*user;

	user = ft_midjoin("~", getenv("USER"), '\0');
	if (ft_lencount(NULL, command, STRS) == 3)
	{
		printf("cd: string not in pwd: %s\n", command[1]);
		shell->exstat = 1;
	}
	else if (ft_lencount(NULL, command, STRS) == 1 || string_compare(command[1], "~", NO_LEN) == 1 || string_compare(command[1], user, NO_LEN) == 1)
		shell->exstat = chdir(getenv("HOME"));
	else if (string_compare(command[1], "-", NO_LEN) == 1)
		shell->exstat = chdir(getenv("OLDPWD"));
	else if (ft_lencount(NULL, command, STRS) == 2)
		shell->exstat = chdir(command[1]);
	else
	{
		printf("cd: too many agurments");
		shell->exstat = 1;
	}
	free(user);
	return (0);
}

char	*ft_stringvalue(char *str)
{
	int		index;
	char	*result;

	index = 0;
	result = (char *)ft_calloc(sizeof(char), ft_lencount(str, NULL, STR) + 1);
	if (result == NULL)
		return (0);
	while (str[index] != '\0')
	{
		result[index] = str[index];
		index++;
	}
	result[index] = '\0';
	return (result);
}

int	execution_command(t_shell *shell, t_execute *exe)
{	
	t_cmd	*cmds;

	cmds = exe->cmds;
	signal_defualt();
	redirect_dup_start(shell, exe);
	// if (string_compare(cmds->tokens[0], "$?", NO_LEN) == 1) //! check agign //
	// 	shell->exstat = 127;
	if (string_compare(cmds->tokens[0], "cd", NO_LEN) == 1)
		exit (EXIT_SUCCESS);
	else if (string_compare(cmds->tokens[0], "export", NO_LEN) == 1)
		exit (EXIT_SUCCESS);
	else if (string_compare(cmds->tokens[0], "unset", NO_LEN) == 1)
		exit (EXIT_SUCCESS);
	else if (string_compare(cmds->tokens[0], "env", NO_LEN) == 1)
		shell->exstat = execution_print_env(shell);
	else if (access(cmds->tokens[0], F_OK | R_OK | X_OK) == 0)
		shell->exstat = execution_token(shell, cmds->tokens[0], cmds->tokens);
	else
		execution_path_command(shell, cmds->tokens);
	exit(shell->exstat);
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
	if (shell->cmds->next != NULL)
		dup2(exe->fd[0], 0);
	else
		dup2(shell->sinput, 0);
	close(exe->fd[0]);
	close(exe->fd[1]);
	if (exe->index == 0 && exe->xedni == 0)
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

	while (shell->cmds != NULL && exe.execute != -42)
	{
		executeion_inite(shell, &exe);
		if (redirect_infile(shell, exe.cmds) < 0)
			return (-1);
		while (exe.index + exe.xedni < exe.files || exe.execute == 0)
		{
			if (pipe(exe.fd) < 0)
				return (-1);
			exe.pid = fork();
			if (exe.pid == 0)
				execution_command(shell, &exe);
			else if (exe.pid > 0)
				exe.execute = execution_waitpid(shell, &exe);
		}
		if (stat(IN_FILE, &exe.info) == 0)
			unlink(IN_FILE);
		if ((exe.cmds->opt == OPT_AND && shell->exstat != 0)
			|| (exe.cmds->opt == OPT_OR && shell->exstat == 0))
			break ;
		shell->cmds = shell->cmds->next;
	}
	return (0);
}
