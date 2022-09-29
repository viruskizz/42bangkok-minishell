/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 20:38:04 by sharnvon          #+#    #+#             */
/*   Updated: 2022/09/29 22:35:22 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execution_path_command(t_shell *shell, char **command, int count, int status)
{
	int		index;
	char	*path;
	char	**env_path;

	env_path = ft_split_mode(getenv("PATH"), ':', BOUND);  //? FREE ?? //
	index = 0;
	while (env_path[index] != NULL)
	{
		path = ft_midjoin(env_path[index++], command[0], '/');
		//* command is exist and can execute */
		if (access(path, F_OK | R_OK | X_OK) == 0)
		{
			status = execution_token(shell, path, command, count);
			if (status == -1)
			{
				//error free and exit;
			}
			//free section//
			free_double_pointer(command, env_path, NULL, path);
			break ;
		}
		//* command is not exist and can't execute */
		else if (env_path[index + 1] == NULL)
		{
			// command not exite : error section//
			printf("minishell: command not found: %s\n", command[0]);
			free_double_pointer(command, env_path, NULL, path);
			return (-1);
		}
	}
	return (1);
}
/* function free 3(double pointer) and 1(pointer) */
/* arg(1 - 3): double pointer | arg(4): pointer */
void	free_double_pointer(char **str1, char **str2, char **str3, char *str4)
{
	int	index;

	index = 0;
	if (str1 != NULL)
	{
		while(str1[index] != NULL)
			free(str1[index++]);
		free(str1);
	}
	index = 0;
	if (str2 != NULL)
	{
		while(str2[index] != NULL)
			free(str2[index++]);
		free(str2);
	}
	index = 0;
	if (str3 != NULL)
	{
		while(str3[index] != NULL)
			free(str3[index++]);
		free(str3);
	}
	if (str4 != NULL)
		free(str4);
}

int	execution_token(t_shell *shell, char *path, char **command, int index)
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
		signal_defualt();
		if (minishell_redirect(shell, fd, index) == -1)
			return (-1);
		execve(path, command, environ);
		printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
	}
	else if (pid > 0)
	{
		waitpid(-1, &status, 0);
		if (shell->tokens[index].redir == FFROM)
			unlink(HERE_DOC);
		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
	}
	return (status);
}

int	cmd_execution(t_shell *shell)
{
	char			**command;
	int				count;
	static int		status = 0;

	count = 0;
	while (count < shell->cmd_amount)
	{	
		command = ft_split_mode(shell->tokens[count].token, ' ', BOUND);
		if (command == NULL)
			return (-1);
		//* bashing command $? show the return command */ /* check command */
		if (string_compare(shell->tokens[count].token, "$?") == 1)
		{
			printf("minishell: command not found: %d\n", status);
			status = 127;
		}

		//* bashing command cd changing directory */
		else if (string_compare(command[0], "cd") == 1)
			status = execution_change_directory(command);
		/*{
			char *user;
			user = ft_midjoin("~", getenv("USER"), '\0');
			if (ft_lencount(NULL, command, STRS) == 3)
				printf("cd: the option isn't avalable\n");
			else if (ft_lencount(NULL, command, STRS) == 1 || string_compare(command[1], "~") == 1 || string_compare(command[1], user) == 1)
				chdir(getenv("HOME"));
			else if (string_compare(command[1], "-") == 1)
				chdir(getenv("OLDPWD"));
			else if (ft_lencount(NULL, command, STRS) == 2)
				chdir(command[1]);
			else
				printf("cd: too many agurments");
			free(user);
		}*/
		
		//* bashing command environment : export, unset, env */
		else if (string_compare(command[0], "export") == 1)
			status = execution_export_env(shell, command);
		else if (string_compare(command[0], "unset") == 1)
			status = execution_unset_env(&shell->env, command[1]);
		else if (string_compare(command[0], "env") == 1)
			status = execution_print_env(shell);


		//* bashing command section */
		//* check and bashing commnad from abslute path */
		else if (access(command[0], F_OK | R_OK | X_OK) == 0)
			status = execution_token(shell, command[0], command, count);
		//* check and bashing command from env_path */
		else
		{
			status = execution_path_command(shell, command, count, status);
			// // status = execution_envpath(shell, command, count, status);
			// // TODO status = execute_envpath()
			// // add "free(str)" in the split //
			// env_path = ft_split_mode(getenv("PATH"), ':', BOUND);  //? FREE ?? //
			// // access: check path //
			// index = 0;
			// while (env_path[index] != NULL)
			// {
			// 	path = ft_midjoin(env_path[index++], command[0], '/');
			// 	//* command is exist and can execute */
			// 	if (access(path, F_OK | R_OK | X_OK) == 0)
			// 	{
			// 		status = execution_token(shell, path, command, count);
			// 		if (status == -1)
			// 		{
			// 			//error free and exit;
			// 		}
			// 		//free section//
			// 		// TODO void	make_it_free
			// 		for (int i = 0; command[i] != NULL; i++)
			// 			free(command[i]);
			// 		free(command);
			// 		for (int i = 0; env_path[i] != NULL; i++)
			// 			free(env_path[i]);
			// 		free(env_path);
			// 		free(path);
			// 		break ;
			// 	}
			// 	//* command is not exist and can't execute */
			// 	else if (env_path[index + 1] == NULL)
			// 	{
			// 		//perror("minishell");
			// 		printf("minishell: command not found: %s\n", command[0]);
			// 		return (-1);
			// 		// command not exite : error section//
			// 		// free section //
			// 	}
			// 	//free(path);
		}
		if ((shell->tokens[count].opt == OPT_AND && status != 0) || (shell->tokens[count].opt == OPT_OR && status == 0))
			break ;
		count++;
	}
	return (0);
}