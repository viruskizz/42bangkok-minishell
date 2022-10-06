/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_redirect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 00:09:42 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/06 16:25:28 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_stringvalue(char *str);

t_env	*convert_env_find(t_env *environment, char *str, int *i)
{
	char	*env_name;
	t_env	*env;
	int		len;

	env = environment;
	while (ft_isalnum(str[(*i + 1) + len]) == 1 || str[(*i + 1) + len] == '_')
		len++;
	env_name = (char *)ft_calloc(sizeof(char), len + 1);
	if (env_name == NULL)
		return (0);
	env_name = string_tranfer(env_name, &str[(*i) + 1], len);
	while (env != NULL)
	{
		if (string_compare(env_name, env->name, NO_LEN) == 1)
		{
			free(env_name);
			return (env);
		}
		env = env->next;
	}
	while ((str[*i] < '\t' || str[*i] > '\r') && str[*i] != ' ')
		(*i)++;
	free(env_name);
	return (NULL);
}

char *heredoc_convert_env(t_env *environment, char *buff, int index, int xedni)
{
	t_env	*env;
	int		count;
	char	*str;

	str = ft_strdup(buff);
	ft_bzero((void *)buff, (size_t)ft_lencount(str, NULL, STR));
	while(str[index] != '\0')
	{
		if (str[index] == '$')
		{
			env = convert_env_find(environment, str, &index);
			if (env != NULL)
			{
				count = 0;
				while (env->value[count] != '\0')
					buff[xedni++] = env->value[count++];
				index = index + ft_lencount(env->name, NULL, STR) + 1;
			}
		}
		else
			buff[xedni++] = str[index++];
	}
	buff[xedni] = '\0';
	free_double_pointer(NULL, NULL, str);
	return (buff);
}

int	heredoc_inputpassing(t_shell *shell, int infile, char *delimiter)
{
	int		read_byte;
	char	*buff;
	t_env	*env;

	buff = (char *)ft_calloc(sizeof(char), 10000);
	if (buff == NULL)
		return (-1);
	while (1)
	{
		env = shell->env;
		write(1, "(heredoc)-> ", 12);
		read_byte = read(0, buff, 9999);
		buff[read_byte] = '\0';
		if (string_compare(buff, delimiter, NO_LEN) == 1)
			break ;
		heredoc_convert_env(shell->env, buff, 0, 0);
		printf("buf = %s| infile =%d\n", buff, infile);
		ft_putstr_fd(buff, infile);
	}
	free(buff);
	return (0);
}

int	redirect_read_heredoc(t_shell *shell, char **files, int infile)//, int fd)
{
	char	*delimiter;
	int		file;
	int		index;

	index = 0;
	if (files == NULL)
		return (0);
	while(files[index] != NULL)
	{
		printf("file[0]: %s, index = %d\n", files[index], index);
		delimiter = ft_midjoin(files[index], "\n", '\0');
		if (delimiter == NULL)
			return (-1);
		if (heredoc_inputpassing(shell, infile, delimiter) == -1)
		{
			free(delimiter);
			return (-1);
		}
		index++;
	}
	//dup2(file, 0);
	free(delimiter);
	return (0);
}

// int	redirect_to_file(t_shell *shell, int index)
// {
// 	int	fd;

// 	if (shell->tokens[index].redir == TO)
// 	{
// 		fd = open(shell->tokens[index].file, O_RDWR | O_CREAT);
// 		if (fd < 0)
// 			return (-1);
// 	}
// 	else
// 	{
// 		fd = open(shell->tokens[index].file, O_RDWR | O_CREAT | O_APPEND, 0777);
// 		if (fd < 0)
// 			return (-1);
// 	}
// 	dup2(fd, 1);
// 	close(fd);
// 	return (0);
// }

// int	minishell_redirect(t_shell *shell, int *fd, int index)
// {
// 	if (shell->tokens->fls != NULL)
// 	{
// 		if (redirect_from_file(shell, index) == -1)
// 			return (-1);
// 	}
// 	else if (shell->tokens->flsls != NULL)
// 	{
// 		if (redirect_from_heredoc(shell, index) == -1)
// 			return (-1);
// 	}
// 	if (shell->tokens->fg != NULL || shell->tokens->fgg == TTO)
// 	{
// 		if (redirect_to_file(shell, index) == -1)
// 			return (-1);
// 	}
// 	else if (index < shell->cmd_amount - 1)
// 		dup2(fd[1], 1);
// 	return (0);
// }

int	redirect_read_file(char **files, int infile, int index)
{
	char	*buff;
	int		file;
	int		read_byte;

	if (files == NULL)
		return (0);
	buff = (char *)ft_calloc(sizeof(char), 100000);
	if (buff == NULL)
		return (-1);
	while (files[index] != NULL)
	{
		file = open(files[index++], O_RDONLY);
		if (file < 0)
			return (-1);
		while (1)
		{
			read_byte = read(file, buff, 9999);
			buff[read_byte] = '\0';
			if (read_byte <= 0)
				break ;
			ft_putstr_fd(buff, infile);
		}
		close(file);
	}
	free(buff);
	return (0);
}

 int	redirect_infile(t_shell *shell, t_cmd *cmds)
{
	int		infile;
	char	*path;
	// t_cmd	*file;

	// file = (t_cmd *)shell->cmds->content;
	// if (file->fls == NULL && file->flsls == NULL)
	// 	return (0);
	path = ft_midjoin(getenv("HOME"), IN_FILE, '/');
	infile = open(path, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (infile < 0)
	{
		perror("minishell: ");
		free(path);
		return (-1);
	}
	if (redirect_read_file(cmds->fls, infile, 0) == -1)
	{
		perror("minishell: ");
		free(path);
		close(infile);
		return (-1);
	}
	// while (files[index] != NULL)
	// {
	// 	file = open(files[index], O_RDONLY);
	// 	printf("file = %d\n", file);
	// 	if (file < 0)
	// 	{
	// 		close(infile);
	// 		//* printf error ?//
	// 		return (-1);
	// 	}
	// 	int count = 0;
	// 	int	read_byte = 0;
	// 	char *buff = (char *)ft_calloc(sizeof(char), 100000);
	// 	if (buff == NULL)
	// 		printf("fuck malloc\n");
	// 	// * redirect_write_file(int infile, int file);
	// 	while (1)
	// 	{
	// 		printf("buff => , file = %d", file);
	// 		read_byte = read(file, buff, 9999);
	// 		buff[read_byte] = '\0';
	// 		if (read_byte <= 0)
	// 			break ;
	// 		ft_putstr_fd(buff, infile);
	// 	}
	// 	free(buff);
	// 	close(file);
	// 	index++;
	// }
	if (redirect_read_heredoc(shell, cmds->flsls, infile) == -1)
	{
		perror("minishell: ");
		free(path);
		close(infile);
		return (-1);
	}
	free(path);
	close(infile);
	return (0);


	// infile = open(path, O_RDONLY);
	// if (infile < 0)
	// {
	// 	perror("minishell: ");
	// 	free(path);
	// }
	// free(path);
	// dup2(infile, 0);
	// close(infile);
	// return (infile);
}

// int	execution_token(t_shell *shell, char *path, char **command)
// {
// 	pid_t	pid;
// 	int 	fd[2];
// 	int		count = 0;
// 	int		outfile = 0;

// 	while (shell->tokens->fg[count] != NULL)
// 		count++;
// 	while (shell->tokens->fgg[outfile] != NULL)
// 		outfile++;
// 	count += outfile;
// 	// * make int file*/
// 	if (shell->tokens->fls != NULL || shell->tokens->flsls != NULL)
// 	{
// 		//* function make infile //
// 	}



// 	while(count > 0)
// 	{
// 		int pi = pipe(fd);
// 		printf("pi = %d\n", pi);
// 		pid = fork();
// 		if (pid < 0 )
// 			return (-1);
// 		else if (pid == 0)
// 		{
// 			signal_defualt();
// 			if (minishell_redirect(shell, fd, index) == -1)
// 				return (-1);
// 			execve(path, command, environ);
// 		}
// 		else if (pid > 0)
// 		{
// 			waitpid(-1, &status, 0);
// 			if (shell->tokens[index].redir == FFROM)
// 				unlink(HERE_DOC);
// 			dup2(fd[0], 0);
// 			close(fd[0]);
// 			close(fd[1]);
// 		}
// 		count--;
// 	}
// 	return (status);
// }

// int	main(void)
// {
// 	int index = 1;
// 	int	fd[2];
// 	t_shell	*shell;

// 	shell = (t_shell *)ft_calloc(sizeof(t_shell), 1);
// 	if (shell == NULL)
// 		return (EXIT_FAILURE);
	
// 	minishell_make_environment(shell);
// 	execution_print_env(shell);

// 	t_cmd	*new;
// 	new = (t_cmd *)ft_calloc(sizeof(t_cmd), 1);
// 	if (new == NULL)
// 		return (0);
// 	// * wc -l <hello1 <hello2 <hello3 // << AAAA <<1111
// 	new->tokens = (char **)ft_calloc(sizeof(char *), 3);
// 	new->tokens[0] = ft_stringvalue("wc");
// 	new->tokens[1] = ft_stringvalue("-l");
// 	new->tokens[2] = NULL;
// 	new->fg = NULL;
// 	new->fgg = NULL;

// 	new->fls = (char **)ft_calloc(sizeof(char *), 4);
// 	new->fls[0] = ft_stringvalue("./hello1");
// 	new->fls[1] = ft_stringvalue("./hello2");
// 	new->fls[2] = ft_stringvalue("./hello3");
// 	new->flsls = (char **)ft_calloc(sizeof(char *), 4);
// 	new->flsls[0] = ft_stringvalue("1111");
// 	new->flsls[1] = ft_stringvalue("2222");
// 	new->flsls[2] = NULL;
// 	new->opt = 0;

// 	ft_lstadd_back(&shell->cmds, ft_lstnew(new));
// 	// printf("=> %s\n", ((char *)((t_lcmd *)shell->cmds->content)->tokens->content));//(char *)tmp->content);
// 	// ((t_lcmd *)shell->cmds->content)->tokens = ((t_lcmd *)shell->cmds->content)->tokens->next;
// 	// printf("=> %s\n", ((char *)((t_lcmd *)shell->cmds->content)->tokens->content));//(char *)tmp->content);


// 	pipe(fd);
// 	printf("hello\n");
// 	if (redirect_infile(shell, fd) == -1)
// 		printf("the file connot open\n");
	
// 	//unlink(".qqqqqqqqqqqqqqqqqqq");
// 	printf("done\n");
// 	return (0);
// }