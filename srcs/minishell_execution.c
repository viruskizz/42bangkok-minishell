/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 20:38:04 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/06 16:26:38 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execution_path_command(t_shell *shell, char **command)
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

/* function free 3(double pointer) and 1(pointer) */
/* arg(1 - 3): double pointer | arg(4): pointer */
// void	free_double_pointer(char **str1, char **str2, char **str3, char *str4)
// {
// 	int	index;

// 	index = 0;
// 	if (str1 != NULL)
// 	{
// 		while(str1[index] != NULL)
// 			free(str1[index++]);
// 		free(str1);
// 	}
// 	index = 0;
// 	if (str2 != NULL)
// 	{
// 		while(str2[index] != NULL)
// 			free(str2[index++]);
// 		free(str2);
// 	}
// 	index = 0;
// 	if (str3 != NULL)
// 	{
// 		while(str3[index] != NULL)
// 			free(str3[index++]);
// 		free(str3);
// 	}
// 	if (str4 != NULL)
// 		free(str4);
// }

// int	execution_token(t_shell *shell, char *path, char **command)
// {
// 	pid_t	pid;
// 	int		shell->exstat;
// 	int 	fd[2];

// 	pipe(fd);
// 	pid = fork();
// 	if (pid < 0 )
// 		return (-1);
// 	else if (pid == 0)
// 	{
// 		signal_defualt();
// 		if (minishell_redirect(shell, fd, index) == -1)
// 			return (-1);
// 		execve(path, command, environ);
// 		printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
// 	}
// 	else if (pid > 0)
// 	{
// 		waitpid(-1, &shell->exstat, 0);
// 		if (shell->tokens[index].redir == FFROM)
// 			unlink(HERE_DOC);
// 		dup2(fd[0], 0);
// 		close(fd[0]);
// 		close(fd[1]);
// 	}
// 	return (shell->exstat);
// }

// int	cmd_execution(t_shell *shell)
// {
// 	// char	**command;
// 	t_list	*cmds;
// 	t_cmd	*command;
// 	int		infile;
// 	int		fd[2];
// 	int		sinput;
// 	int		soutput;

// 	sinput = dup(0);
// 	soutput = dup(1);
	
// 	cmds = shell->cmds;
// 	// printf("==> %s\n", ((t_cmd *)cmds->content)->tokens[0]);
// 	// exit (0);
// 	while (cmds != NULL)
// 	{
// 		command = (t_cmd *)cmds->content;
// 		// ? manage redirect in_file//
// 		if (command->fls != NULL && command->flsls != NULL)
// 			infile = redirect_infile();
// 		// * int index = 0; int xedni = 0;
// 		// * while following the outfile amount // while (command->fg[index] != NULL || command->fgg[xedni] != NULL)
// 		// ? manage redirect out_file// 
// 		pipe(fd);

// 		//* bashing command $? show the return command */ /* check command */
// 		if (string_compare(command->tokens[0], "$?") == 1) //! check agign //
// 		{
// 			printf("minishell: command not found: %d\n", shell->exstat);
// 			shell->exstat = 127;
// 		}
// 		//* bashing command cd changing directory */
// 		else if (string_compare(command->tokens[0], "cd") == 1)
// 			execution_change_directory(shell, command->tokens);
// 			// shell->exstat = execution_change_directory(shell->tokens->tokens);
// 		/*{
// 			char *user;
// 			user = ft_midjoin("~", getenv("USER"), '\0');
// 			if (ft_lencount(NULL, command, STRS) == 3)
// 				printf("cd: the option isn't avalable\n");
// 			else if (ft_lencount(NULL, command, STRS) == 1 || string_compare(command[1], "~") == 1 || string_compare(command[1], user) == 1)
// 				chdir(getenv("HOME"));
// 			else if (string_compare(command[1], "-") == 1)
// 				chdir(getenv("OLDPWD"));
// 			else if (ft_lencount(NULL, command, STRS) == 2)
// 				chdir(command[1]);
// 			else
// 				printf("cd: too many agurments");
// 			free(user);
// 		}*/
		
// 		//* bashing command environment : export, unset, env */
// 		else if (string_compare(command->tokens[0], "export") == 1)
// 			shell->exstat = execution_export_env(shell, command->tokens);
// 		else if (string_compare(command->tokens[0], "unset") == 1)
// 			shell->exstat = execution_unset_env(&shell->env, command->tokens);
// 		else if (string_compare(command->tokens, "env") == 1)
// 			shell->exstat = execution_print_env(shell);


// 		//* bashing command section */
// 		//* check and bashing commnad from abslute path */
// 		else if (access(command->tokens[0], F_OK | R_OK | X_OK) == 0)
// 			shell->exstat = execution_token(shell, command->tokens[0], command->tokens);
// 		//* check and bashing command from env_path */
// 		else
// 		{
// 			shell->exstat = execution_path_command(shell, command->tokens);
// 			// // shell->exstat = execution_envpath(shell, command, count, shell->exstat);
// 			// // TODO shell->exstat = execute_envpath()
// 			// // add "free(str)" in the split //
// 			// env_path = ft_split_mode(getenv("PATH"), ':', BOUND);  //? FREE ?? //
// 			// // access: check path //
// 			// index = 0;
// 			// while (env_path[index] != NULL)
// 			// {
// 			// 	path = ft_midjoin(env_path[index++], command[0], '/');
// 			// 	//* command is exist and can execute */
// 			// 	if (access(path, F_OK | R_OK | X_OK) == 0)
// 			// 	{
// 			// 		shell->exstat = execution_token(shell, path, command, count);
// 			// 		if (shell->exstat == -1)
// 			// 		{
// 			// 			//error free and exit;
// 			// 		}
// 			// 		//free section//
// 			// 		// TODO void	make_it_free
// 			// 		for (int i = 0; command[i] != NULL; i++)
// 			// 			free(command[i]);
// 			// 		free(command);
// 			// 		for (int i = 0; env_path[i] != NULL; i++)
// 			// 			free(env_path[i]);
// 			// 		free(env_path);
// 			// 		free(path);
// 			// 		break ;
// 			// 	}
// 			// 	//* command is not exist and can't execute */
// 			// 	else if (env_path[index + 1] == NULL)
// 			// 	{
// 			// 		//perror("minishell");
// 			// 		printf("minishell: command not found: %s\n", command[0]);
// 			// 		return (-1);
// 			// 		// command not exite : error section//
// 			// 		// free section //
// 			// 	}
// 			// 	//free(path);
// 		}
// 		if ((command->opt == OPT_AND && shell->exstat != 0) || (command->opt == OPT_OR && shell->exstat == 0))
// 			break ;
// 		cmds = cmds->next;//count++; // shell->cmds = shell->cmds->next;
// 	}
// 	return (0);
// }

int	redirect_dup_input(int *fd)
{
	int	infile;

	infile = open(IN_FILE, O_RDONLY);
	if (infile < 0)
	{
		dup2(fd[0], 0);
		close(fd[0]);
		return (0);
	}
	dup2(infile, 0);
	close(infile);
	return (0);
}

int redirect_dup_start(t_shell *shell, int *index, int *xedni)
{
	int		fd[2];
	int		file;
	t_cmd	*command;

	command = (t_cmd *)shell->cmds;
	if (pipe(fd) == -1)
		return (-1);
	redirect_dup_input(fd);
	if (command->fg[(*index)] == NULL &&command->fgg[(*xedni)] == NULL && command->opt != 0)
		dup2(fd[1], 1);
	else if (command->fg[(*index)] != NULL || command->fgg[(*xedni)] != NULL)
	{
		if (command->fg[(*index)] != NULL)
			file = open(command->fg[(*index)++], O_RDWR | O_CREAT, 0644);
		else
			file = open(command->fgg[(*xedni)++], O_RDWR | O_CREAT | O_APPEND, 0644);
		if (file < -1)
			return (-1);
		dup2(file, 1);
		close(file);
	}
	close(fd[1]);
	return (0);
}

int	cmd_execution(t_shell *shell)
{
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
	while (shell->cmds != NULL)
	{
		cmds = (t_cmd *)shell->cmds->content;
		if (cmds->fls != NULL && cmds->flsls != NULL)
			redirect_infile(shell, cmds);
			while (cmds->fg[index] != NULL || cmds->fgg[xedni] != NULL)
			{
				redirect_dup_start(shell, &index, &xedni);
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
					shell->exstat = execution_path_command(shell, cmds->tokens);
				// *dup2 back ;
				dup2(sinput, 0);
				dup2(soutput, 1);
			}
		if ((cmds->opt == OPT_AND && shell->exstat != 0) || (cmds->opt == OPT_OR && shell->exstat == 0))
			break ;
		shell->cmds = shell->cmds->next;
	}
	return (0);
}