/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution_befor_fork.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 20:38:04 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/08 20:45:17 by sharnvon         ###   ########.fr       */
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
			shell->exstat = execution_token(shell, path, command);
			if (shell->exstat == -1)
			{
				//error free and exit;
			}
			//free section//
			free_double_pointer(command, env_path, path);
			break ;
		}
		//* command is not exist and can't execute */
		else if (env_path[index + 1] == NULL)
		{
			// command not exist : error section//
			printf("minishell: command not found: %s\n", command[0]);
			free_double_pointer(command, env_path, path);
			return (-1);
		}
	}
	return (1);
}

int	execution_token(t_shell *shell, char *path, char **command)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0 )
		return (-1);
	else if (pid == 0)
	{
		signal_defualt();
		execve(path, command, NULL);
	}
	else if (pid > 0)
	{
		waitpid(-1, &shell->exstat, 0);
	}
	return (shell->exstat);
}

int	redirect_dup_input(int *fd)
{
	int		infile;

	infile = open(IN_FILE, O_RDONLY);
	printf("infile = %d\n", infile);
	if (infile < 0)
	{
		printf("dupdup_input\n");
		dup2(fd[0], 0);
		close(fd[0]);
		return (0);
	}
	dup2(infile, 0);
	close(infile);
	close(fd[0]);
	return (0);
}

int redirect_dup_start(t_shell *shell, int *index, int *xedni)
{
	int		fd[2];
	int		file;
	t_cmd	*command;
	
	file = 0;
	command = (t_cmd *)shell->cmds->content;
	if (pipe(fd) == -1)
		return (-1);
	redirect_dup_input(fd);
	if (command->fg == NULL && command->fgg == NULL && command->opt != 0)
	{
		printf("dupdup to pipe\n");
		dup2(fd[1], 1);
	}
	else if (command->fg != NULL && command->fg[(*index)] != NULL)
		file = open(command->fg[(*index)++], O_RDWR | O_CREAT, 0644);
	else if ((command->fgg != NULL && command->fgg[(*xedni)] != NULL))
		file = open(command->fgg[(*xedni)++], O_RDWR | O_CREAT | O_APPEND, 0644);
	if (file < -1)
			return (-1);
	else if (file > 3)
	{
		dup2(file, 1);
		close(file);
	}
	close(fd[1]);
	// close(fd[0]);
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

int	cmd_execution(t_shell *shell)
{
	t_cmd	*test;
	// char	**command;
	t_cmd	*cmds;
	int		sinput;
	int		soutput;
	int		index;
	int		xedni;
	index = 0;
	xedni = 0;
	sinput = dup(0);
	soutput = dup(1);

	test = (t_cmd *)shell->cmds->content;
	printf("%s", YELLOW);
	printf("\n fg: %s\n fgg: %s\n fls: %s\n flsls: %s\n opt = %d\n\n", test->fg, test->fgg, test->fls, test->flsls, test->opt);
	printf("%s", RESET);

	while (shell->cmds != NULL)
	{
		int	count = 0;
		cmds = (t_cmd *)shell->cmds->content;
		if (cmds->fls != NULL || cmds->flsls != NULL)
		{
			printf("%smake??(infile)\n", GREEN);
			redirect_infile(shell, cmds);
			printf("done(infile)%s\n", RESET);
		}
		while ((cmds->fg != NULL && cmds->fg[index] != NULL) || (cmds->fgg != NULL && cmds->fgg[xedni] != NULL) || count == 0)
		{
			printf("%sbefor(dup_start)\n", YELLOW);
			redirect_dup_start(shell, &index, &xedni);
			printf("after(dup_start)%s\n\n", RESET);
			//* bashing command $? show the return command */ /* check command */
			if (string_compare(cmds->tokens[0], "$?", NO_LEN) == 1) //! check agign //
			{
				printf("minishell: command not found: %d\n", shell->exstat);
				shell->exstat = 127;
			}
			//* bashing command cd changing directory */
			else if (string_compare(cmds->tokens[0], "cd", NO_LEN) == 1)
				execution_change_directory(shell, cmds->tokens);
			
			//* bashing command environment : export, unset, env */
			else if (string_compare(cmds->tokens[0], "export", NO_LEN) == 1) // * can do more than 1 arg
				shell->exstat = execution_export_env(shell, cmds->tokens);
			else if (string_compare(cmds->tokens[0], "unset", NO_LEN) == 1) // * can do more than 1 arg
				shell->exstat = execution_unset_env(&shell->env, cmds->tokens);
			else if (string_compare(cmds->tokens[0], "env", NO_LEN) == 1)
				shell->exstat = execution_print_env(shell);

			//* bashing command section */
			//* check and bashing commnad from abslute path */
			else if (access(cmds->tokens[0], F_OK | R_OK | X_OK) == 0)
				shell->exstat = execution_token(shell, cmds->tokens[0], cmds->tokens);
			//* check and bashing command from env_path */
			else
			{
				shell->exstat = execution_path_command(shell, cmds->tokens);
			}
			// *dup2 back ;
			dup2(sinput, 0);
			dup2(soutput, 1);
			// * unlink
			if (open(IN_FILE, O_RDONLY) > 0)
			{
				printf("unlink\n");
				unlink(IN_FILE);
			}
			count++;
		}
		if ((cmds->opt == OPT_AND && shell->exstat != 0) || (cmds->opt == OPT_OR && shell->exstat == 0))
			break ;
		shell->cmds = shell->cmds->next;
	}
	return (0);
}