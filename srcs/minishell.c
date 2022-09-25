/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 00:28:26 by sharnvon          #+#    #+#             */
/*   Updated: 2022/09/26 02:15:07 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	it_need_to_be_free()
{

}
// void	sig_handler(int signum)
// {
// 	printf("=> %d\n", signum);
// 	if (signum == SIGINT)
// 		printf("SIGIN IS PUSHED\n");
// 	if (signum == SIGQUIT)
// 		printf("SIGKILL IS PUSHED\n");
// 	if (signum == EOF)
// 		printf("EOF IS PUSHED\n");
// }
// void	signal_register(void)
// {
// 	struct sigaction	signal;

// 	sigemptyset(&signal.sa_mask);
// 	signal.sa_flags = 0;
// 	signal.sa_handler = &sig_handler;
// 	sigaction(SIGINT, &signal, NULL);
// 	// sigaction(SIGQUIT, &signal, NULL);
// 	sigaction(EOF, &signal, NULL);
// }

void	signal_defualt(void)
{
	struct sigaction	signal;

	sigemptyset(&signal.sa_mask);
	signal.sa_flags = 0;
	signal.sa_handler = SIG_DFL;
	sigaction(SIGINT, &signal, NULL);
	sigaction(SIGQUIT, &signal, NULL);
}

int	execute_now(t_shell *shell, char *path, char **command, int index)
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
		// if (shell->tokens[index].redir == FROM)
		// {
		// 	from_fd = open(shell->tokens[index].file, O_RDWR);
		// 	dup2(from_fd, 0);
		// 	close(from_fd);
		// }
		// else if (shell->tokens[index].redir == FFROM)
		// {
		// 	int		read_byte;
		// 	char	*buff;
		// 	char	*delimiter;
		// 	delimiter = ft_midjoin(shell->tokens[index].file, "\n", '\0');
		// 	// if (delimiter == NULL)
		// 		//errror //
		// 	buff = (char *)ft_calloc(sizeof(char), 10000);
		// 	if (buff == NULL)
		// 		printf("malloc_fuck\n");
		// 	from_fd = open(HERE_DOC, O_RDWR | O_CREAT | O_APPEND, 0777);
		// 	if (from_fd < 0)
		// 		printf("fd_fuck\n");
		// 	while (1)
		// 	{
		// 		read_byte = read(0, buff, 9999);
		// 		buff[read_byte] = '\0';
		// 		if (string_compare(buff, delimiter) == 1)
		// 			break ;
		// 		// need to convert the env variable //
		// 		ft_putstr_fd(buff, from_fd);
		// 	}
		// 	close(from_fd);
		// 	from_fd = open(HERE_DOC, O_RDWR | O_CREAT | O_APPEND, 0777);
		// 	dup2(from_fd, 0);
		// 	close(from_fd);
		// 	free(buff);
		// 	free(delimiter);
		// }
		// if (shell->tokens[index].redir == TO || shell->tokens[index].redir == TTO)
		// {
		// 	if (shell->tokens[index].redir == TO)
		// 		to_fd = open(shell->tokens[index].file, O_RDWR | O_CREAT);
		// 	else
		// 		to_fd = open(shell->tokens[index].file, O_RDWR | O_CREAT | O_APPEND, 0777);
		// 	dup2(to_fd, 1);
		// 	close(to_fd);
		// }
		// else if (index < shell->cmd_amount - 1) // * add cordition < << > >> //
		// 	dup2(fd[1], 1);
		// close(fd[0]);
		// close(fd[1]);
		execve(path, command, environ);
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
	status = 0;
	while (count < shell->cmd_amount)
	{	
		command = ft_split_mode(shell->tokens[count].token, ' ', BOUND);
		if (command == NULL)
			return (-1);
		//* bashing command $? show the return command */
		if (string_compare(shell->tokens[count].token, "$?") == 1)
			printf("minishell: command not found: %d\n", status);
		//* bashing command cd changing directory */
		else if (string_compare(command[0], "cd") == 1)
		{
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
		}
		//* bashing command section */
			//* check and execute abslute path */
		else if (access(command[0], F_OK | R_OK | X_OK) == 0)
			status = execute_now(shell, command[0], command, count);
			//* check and execute from env_path */
		else
		{
			// TODO status = execute_envpath()
			// add "free(str)" in the split //
			env_path = ft_split_mode(getenv("PATH"), ':', BOUND);  //? FREE ?? //
			// access: check path //
			index = 0;
			while (env_path[index] != NULL)
			{
				path = ft_midjoin(env_path[index++], command[0], '/');
				//* command is exist and can execute */
				if (access(path, F_OK | R_OK | X_OK) == 0)
				{
					status = execute_now(shell, path, command, count);
					if (status == -1)
					{
						//error free and exit;
					}
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
		}
		if ((shell->tokens[count].opt == OPT_AND && status != 0) || (shell->tokens[count].opt == OPT_OR && status == 0))
			break ;
		count++;
	}
	return (0);
}

int	test_execution(void)
{
	// line = ls -la | wc -l
	t_shell		*shell;
	char		dir[100];
	char		*dir_path;

	// * ls -l | wc -l 
	// signal_register();
	shell = (t_shell *)ft_calloc(sizeof(t_shell), 1);
	if (shell == NULL)
		return (EXIT_FAILURE);
	shell->tokens = (t_token *)ft_calloc(sizeof(t_token), 3);
	if (shell->tokens == NULL)
		return (0);
	shell->tokens[0].token = ft_stringvalue("echo $?");
	shell->tokens[0].opt = OPT_NULL;
	shell->tokens[0].redir = 0;
	shell->tokens[0].file = NULL;

	shell->tokens[1].token = ft_stringvalue("wc -l");
	shell->tokens[1].opt = OPT_PIPE;

	shell->tokens[2].token = ft_stringvalue("wc -c");
	shell->tokens[2].opt = OPT_NULL;
	shell->tokens[2].redir = TTO;
	shell->tokens[2].file = "test_tto.txt";
	shell->cmd_amount = 1;
	
	// while (1);
	cmd_execution(shell);
 	printf("\ncurrent directory is \"%s\"\n",getcwd(dir, 100));
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
