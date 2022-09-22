/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 00:28:26 by sharnvon          #+#    #+#             */
/*   Updated: 2022/09/23 01:44:54 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execution_now(t_shell *shell, char *path, char **command, int index)
{
	pid_t	pid;
	int		status;
	int 	fd[2];

	pipe(fd);
	pid = fork();
	if (pid < 0 )
		return (-1);
	else if (pid == 0)
	{
		if (index < shell->cmd_amount - 1)
			dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		execve(path, command, environ);
	}
	else if (pid > 0)
	{
		waitpid(-1, &status, 0);
		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
	}
	return (status);
}

char	*ft_stringvalue(char *str)
{
	int		index;
	char	*result;

	index = 0;
	result = (char *)ft_calloc(sizeof(char), ft_lencount(str) + 1);
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
	char	**command;
	char	**env_path;
	char	*path;
	int		count;
	int		index;
	int		status;

	count = 0;
	while (count < shell->cmd_amount)
	{	
		command = ft_split(shell->cmds[count].cmd, ' ', BOUND);
		if (command == NULL)
			return (-1);

		//* bashing command section */
			//* check and execute abslute path */
		if (access(command[0], F_OK | R_OK | X_OK) == 0)
			status = execution_now(shell, command[0], command, count);
			//* check and execute from env_path */
		else
		{
			// add "free(str)" in the split //
			env_path = ft_split(getenv("PATH"), ':', BOUND);  //? FREE ?? //
			// access: check path //
			index = 0;
			while (env_path[index] != NULL)
			{
				path = ft_strjoin(env_path[index++], command[0], '/');
				//* command is exist and can execute */
				if (access(path, F_OK | R_OK | X_OK) == 0)
				{
					status = execution_now(shell, path, command, count);
					//free section//
					for (int i = 0; command[i] != NULL; i++)
						free(command[i]);
					free(command);
					for (int i = 0; env_path[i] != NULL; i++)
						free(env_path[i]);
					free(env_path);
					free(path);
					break ;
				}
				//* command is not exist and can't execute */
				else if (env_path[index + 1] == NULL)
				{
					//perror("minishell");
					printf("minishell: command not found: %s\n", command[0]);
					return (-1);
					// command not exite : error section//
					// free section //

				}
				//free(path);
			}
		}
		count++;
	}
	return (0);
}

int	main(void)
{
	// line = ls -la | wc -l
	t_shell		*shell;

	shell = (t_shell *)ft_calloc(sizeof(t_shell), 1);
	if (shell == NULL)
		return (EXIT_FAILURE);
	shell->cmds = (t_cmd *)ft_calloc(sizeof(t_cmd), 3);
	if (shell->cmds == NULL)
		return (0);
	shell->cmds[0].cmd = ft_stringvalue("ls -la");
	shell->cmds[0].conj = CONJ_PIPE;
	shell->cmds[1].cmd = ft_stringvalue("dfdfdfdf dfdf");
	shell->cmds[1].conj = CONJ_PIPE;
	shell->cmds[2].cmd = ft_stringvalue("wc -c");
	shell->cmds[2].conj = CONJ_NULL;
	shell->cmd_amount = 3;
	cmd_execution(shell);


	return (EXIT_SUCCESS);
}
/*
int	main(void)
{
	char		*line;
	char		*path;

	line = (char *)ft_calloc(sizeof(char), 1000000);
	if (line == NULL)
		return (EXIT_FAILURE);
	while(string_compare(line, "exit") == 0)
	{
		line = readline("\x1B[5m\033[0;35mMI\033[0;36mNI\033[0;34mS\033[0;32mH\033[0;33mE\033[48;5;208mL\033[0;31mL\033[0m--> \x1B[0m");
		add_history(line);
//		printf("%s\n", line);
		ft_bash_command(line);
		printf("ending\n");

	}
	return (EXIT_SUCCESS);
}*/
