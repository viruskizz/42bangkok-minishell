/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 00:28:26 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/04 23:13:42 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_defualt(void)
{
	struct sigaction	signal;

	sigemptyset(&signal.sa_mask);
	signal.sa_flags = 0;
	signal.sa_handler = SIG_DFL;
	sigaction(SIGINT, &signal, NULL);
	sigaction(SIGQUIT, &signal, NULL);
}

// int	execution_token(t_shell *shell, char *path, char **command, int index)
// {
// 	pid_t	pid;
// 	int		status;
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
// 		// if (shell->tokens[index].redir == FROM)
// 		// {
// 		// 	from_fd = open(shell->tokens[index].file, O_RDWR);
// 		// 	dup2(from_fd, 0);
// 		// 	close(from_fd);
// 		// }
// 		// else if (shell->tokens[index].redir == FFROM)
// 		// {
// 		// 	int		read_byte;
// 		// 	char	*buff;
// 		// 	char	*delimiter;
// 		// 	delimiter = ft_midjoin(shell->tokens[index].file, "\n", '\0');
// 		// 	// if (delimiter == NULL)
// 		// 		//errror //
// 		// 	buff = (char *)ft_calloc(sizeof(char), 10000);
// 		// 	if (buff == NULL)
// 		// 		printf("malloc_fuck\n");
// 		// 	from_fd = open(HERE_DOC, O_RDWR | O_CREAT | O_APPEND, 0777);
// 		// 	if (from_fd < 0)
// 		// 		printf("fd_fuck\n");
// 		// 	while (1)
// 		// 	{
// 		// 		read_byte = read(0, buff, 9999);
// 		// 		buff[read_byte] = '\0';
// 		// 		if (string_compare(buff, delimiter) == 1)
// 		// 			break ;
// 		// 		// need to convert the env variable //
// 		// 		ft_putstr_fd(buff, from_fd);
// 		// 	}
// 		// 	close(from_fd);
// 		// 	from_fd = open(HERE_DOC, O_RDWR | O_CREAT | O_APPEND, 0777);
// 		// 	dup2(from_fd, 0);
// 		// 	close(from_fd);
// 		// 	free(buff);
// 		// 	free(delimiter);
// 		// }
// 		// if (shell->tokens[index].redir == TO || shell->tokens[index].redir == TTO)
// 		// {
// 		// 	if (shell->tokens[index].redir == TO)
// 		// 		to_fd = open(shell->tokens[index].file, O_RDWR | O_CREAT);
// 		// 	else
// 		// 		to_fd = open(shell->tokens[index].file, O_RDWR | O_CREAT | O_APPEND, 0777);
// 		// 	dup2(to_fd, 1);
// 		// 	close(to_fd);
// 		// }
// 		// else if (index < shell->cmd_amount - 1) // * add cordition < << > >> //
// 		// 	dup2(fd[1], 1);
// 		// close(fd[0]);
// 		// close(fd[1]);
// 		execve(path, command, environ);
// 	}
// 	else if (pid > 0)
// 	{
// 		waitpid(-1, &status, 0);
// 		if (shell->tokens[index].redir == FFROM)
// 			unlink(HERE_DOC);
// 		dup2(fd[0], 0);
// 		close(fd[0]);
// 		close(fd[1]);
// 	}
// 	return (status);
// }

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

// int	cmd_execution(t_shell *shell)
// {
// 	char	**command;
// 	char	**env_path;
// 	char	*path;
// 	int		count;
// 	int		index;
// 	static int		status;
// 	count = 0;
// 	status = 0;
// 	while (count < shell->cmd_amount)
// 	{	
// 		command = ft_split_mode(shell->tokens[count].token, ' ', BOUND);
// 		if (command == NULL)
// 			return (-1);
// 		//* bashing command $? show the return command */
// 		if (string_compare(shell->tokens[count].token, "$?") == 1)
// 		{
// 			printf("minishell: command not found: %d\n", status);
// 			status = 127;
// 		}
// 		//* bashing command cd changing directory */
// 		else if (string_compare(command[0], "cd") == 1)
// 			status = execution_change_directory(command);
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
// 		//else if (string_compare(command[0], "export") == 1 || string_compare(command[0], "unset") == 1)
// 			//status = excecution_set_enviroment(command);
// 		//* bashing command section */
// 		//* check and bashing commnad from abslute path */
// 		else if (access(command[0], F_OK | R_OK | X_OK) == 0)
// 			status = execution_token(shell, command[0], command, count);
// 		//* check and bashing command from env_path */
// 		else
// 		{
// 			// status = execution_envpath(shell, command, count, status);
// 			// TODO status = execute_envpath()
// 			// add "free(str)" in the split //
// 			env_path = ft_split_mode(getenv("PATH"), ':', BOUND);  //? FREE ?? //
// 			// access: check path //
// 			index = 0;
// 			while (env_path[index] != NULL)
// 			{
// 				path = ft_midjoin(env_path[index++], command[0], '/');
// 				//* command is exist and can execute */
// 				if (access(path, F_OK | R_OK | X_OK) == 0)
// 				{
// 					status = execution_token(shell, path, command, count);
// 					if (status == -1)
// 					{
// 						//error free and exit;
// 					}
// 					//free section//
// 					// TODO void	make_it_free
// 					for (int i = 0; command[i] != NULL; i++)
// 						free(command[i]);
// 					free(command);
// 					for (int i = 0; env_path[i] != NULL; i++)
// 						free(env_path[i]);
// 					free(env_path);
// 					free(path);
// 					break ;
// 				}
// 				//* command is not exist and can't execute */
// 				else if (env_path[index + 1] == NULL)
// 				{
// 					//perror("minishell");
// 					printf("minishell: command not found: %s\n", command[0]);
// 					return (-1);
// 					// command not exite : error section//
// 					// free section //
// 				}
// 				//free(path);
// 			}
// 		}
// 		if ((shell->tokens[count].opt == OPT_AND && status != 0) || (shell->tokens[count].opt == OPT_OR && status == 0))
// 			break ;
// 		count++;
// 	}
// 	return (0);
// }

// int	main(void)
// {
// 	// line = ls -la | wc -l
// 	t_shell		*shell;
// 	char		dir[100];
// 	char		*dir_path;

// 	// * wc -l < hello1 < hello2 < hello3
// 	// signal_register();
// 	// shell = (t_shell *)ft_calloc(sizeof(t_shell), 1);
// 	// if (shell == NULL)
// 	// 	return (EXIT_FAILURE);
// 	// shell->tokens = (t_token *)ft_calloc(sizeof(t_token), 1);
// 	// if (shell->tokens == NULL)
// 	// 	return (0);
// 	// shell->tokens->tokens = (char **)ft_calloc(sizeof(char), 3);
// 	// if (shell->tokens->tokens == NULL)
// 	// 	return (0);
// 	// shell->tokens->tokens[0] = ft_stringvalue("wc -l");
// 	// shell->tokens->fg = NULL;
// 	// shell->tokens->fgg = NULL;
// 	// shell->tokens->fls = NULL;
// 	// shell->tokens->flsls = NULL;
// 	// shell->tokens->opt = 0;

// 	shell = (t_shell *)ft_calloc(sizeof(t_shell), 1);
// 	if (shell == NULL)
// 		return (EXIT_FAILURE);
	
// 	t_cmd	*new;
// 	new = (t_cmd *)ft_calloc(sizeof(t_cmd), 1);
// 	if (new == NULL)
// 		return (0);

// 	new->tokens = (char **)ft_calloc(sizeof(char *), 3);
// 	new->tokens[0] = ft_stringvalue("wc");
// 	new->tokens[1] = ft_stringvalue("-l");
// 	new->tokens[2] = NULL;
// 	new->fg = NULL;
// 	new->fgg = NULL;
// 	new->fls = NULL;
// 	new->flsls = NULL;
// 	new->opt = 0;

// 	ft_lstadd_back(&shell->cmds, ft_lstnew(new));
// 	// printf("=> %s\n", ((char *)((t_lcmd *)shell->cmds->content)->tokens->content));//(char *)tmp->content);
// 	// ((t_lcmd *)shell->cmds->content)->tokens = ((t_lcmd *)shell->cmds->content)->tokens->next;
// 	// printf("=> %s\n", ((char *)((t_lcmd *)shell->cmds->content)->tokens->content));//(char *)tmp->content);

// 	cmd_execution(shell);
//  	printf("\ncurrent directory is \"%s\"\n",getcwd(dir, 100));
// 	return (EXIT_SUCCESS);
// }
