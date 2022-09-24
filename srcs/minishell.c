/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 00:28:26 by sharnvon          #+#    #+#             */
/*   Updated: 2022/09/24 04:46:23 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_now(t_shell *shell, char *path, char **command, int index)
{
	pid_t	pid;
	int		status;
	int 	fd[2];
	int		from_fd;
	int		to_fd;

	pipe(fd);
	pid = fork();
	if (pid < 0 )
		return (-1);
	else if (pid == 0)
	{
		// file_fd = open("test.txt", O_RDONLY); //* redirect with <
		// dup2(file_fd, 0);
		// close(file_fd);
		if (shell->cmds[index].redir == FROM)
		{
			from_fd = open(shell->cmds[index].file, O_RDWR);
			dup2(from_fd, 0);
			close(from_fd);
		}
		if (shell->cmds[index].redir == TO || shell->cmds[index].redir == TTO)
		{
			if (shell->cmds[index].redir == TO)
				to_fd = open(shell->cmds[index].file, O_RDWR | O_CREAT);
			else
				to_fd = open(shell->cmds[index].file, O_RDWR | O_CREAT | O_APPEND, 0777);
			dup2(to_fd, 1);
			close(to_fd);
		}
		else if (index < shell->cmd_amount - 1) // * add cordition < << > >> //
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
			status = execute_now(shell, command[0], command, count);
			//* check and execute from env_path */
		else
		{
			// TODO status = execute_envpath()
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
					status = execute_now(shell, path, command, count);
					//free section//
					// TODO void	make_it_free
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
			if ((shell->cmds[count].conj == CONJ_AND && status != 0) || (shell->cmds[count].conj == CONJ_OR && status == 0))
				break ;
		}
		count++;
	}
	return (0);
}

int	main(void)
{
	// line = ls -la | wc -l
	t_shell		*shell;
	char		dir[100];
	char		*dir_path;

	// unlink("/Users/shivarakii/Documents/42_coding/real_minishell/test.c");
	// printf("%s\n",getcwd(dir, 100));
	// printf("dir = %s | size = %lu\n", dir, sizeof(dir));
	// dir_path = ft_strjoin(dir, "test/", '/');
	// printf("=> %s\n", dir_path);
	// printf("return %d\n", chdir(dir_path));
	// printf("%s\n",getcwd(dir, 100));
	// exit (0);

	// * ls -l | wc -l 
	shell = (t_shell *)ft_calloc(sizeof(t_shell), 1);
	if (shell == NULL)
		return (EXIT_FAILURE);
	shell->cmds = (t_cmd *)ft_calloc(sizeof(t_cmd), 3);
	if (shell->cmds == NULL)
		return (0);
	shell->cmds[0].cmd = ft_stringvalue("wc -l");
	shell->cmds[0].conj = CONJ_PIPE;
	shell->cmds[0].redir = FROM;
	shell->cmds[0].file = "test_from.txt";

	shell->cmds[1].cmd = ft_stringvalue("wc -l");
	shell->cmds[1].conj = CONJ_PIPE;

	shell->cmds[2].cmd = ft_stringvalue("wc -c");
	shell->cmds[2].conj = CONJ_NULL;
	shell->cmds[2].redir = TTO;
	shell->cmds[2].file = "test_tto.txt";
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
