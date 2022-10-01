/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_redirect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 00:09:42 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/01 15:41:55 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_inputpassing(char *buff, char *delimiter)
{
	int	read_byte;
	int	fd;

	fd = open(HERE_DOC, O_RDWR | O_CREAT | O_APPEND, 0777);
	if (fd < 0)
		return (-1);
	while (1)
	{
		read_byte = read(0, buff, 9999);
		buff[read_byte] = '\0';
		if (string_compare(buff, delimiter) == 1)
			break ;
		/* need to convert the env variable */
		ft_putstr_fd(buff, fd);
	}
	close(fd);
	fd = open(HERE_DOC, O_RDWR | O_CREAT | O_APPEND, 0777);
	if (fd < 0)
		return (-1);
	return (fd);
}

int	redirect_from_heredoc(t_shell *shell)//, int fd)
{
	char	*buff;
	char	*delimiter;
	int		fd;
	int		index;

	index = 0;
	buff = (char *)ft_calloc(sizeof(char), 10000);
	if (buff == NULL)
		return (-1);
	while(shell->tokens->flsls[index] != NULL)
	{
		delimiter = ft_midjoin(shell->tokens->flsls, "\n", '\0');
		if (delimiter == NULL)
		{
			free(buff);
			return (-1);
		}
		fd = heredoc_inputpassing(buff, delimiter);
		if (fd < 0)
		{
			free(delimiter);
			free(buff);
			return (-1);
		}
		index++;
	}
	dup2(fd, 0);
	close(fd);
	free(buff);
	free(delimiter);
	return (0);
}

int	redirect_from_file(t_shell *shell, int index)
{
	int	fd;

	fd = open(shell->tokens[index].file, O_RDWR);
	if (fd < 0)
		return (-1);
	dup2(fd, 0);
	close(fd);
	return (0);
}

int	redirect_to_file(t_shell *shell, int index)
{
	int	fd;

	if (shell->tokens[index].redir == TO)
	{
		fd = open(shell->tokens[index].file, O_RDWR | O_CREAT);
		if (fd < 0)
			return (-1);
	}
	else
	{
		fd = open(shell->tokens[index].file, O_RDWR | O_CREAT | O_APPEND, 0777);
		if (fd < 0)
			return (-1);
	}
	dup2(fd, 1);
	close(fd);
	return (0);
}

int	minishell_redirect(t_shell *shell, int *fd, int index)
{
	if (shell->tokens->fls != NULL)
	{
		if (redirect_from_file(shell, index) == -1)
			return (-1);
	}
	else if (shell->tokens->flsls != NULL)
	{
		if (redirect_from_heredoc(shell, index) == -1)
			return (-1);
	}
	if (shell->tokens->fg != NULL || shell->tokens->fgg == TTO)
	{
		if (redirect_to_file(shell, index) == -1)
			return (-1);
	}
	else if (index < shell->cmd_amount - 1)
		dup2(fd[1], 1);
	return (0);
}

int	redirect_make_infile(t_shell *shell, int *fd)
{
	int	index;
	int	infile;
	int	file;

	index = 0;
	infile = open("IN_FILE", O_RDWR | O_CREAT | O_APPEND, 0644);
	if (infile < 0)
		return (-1);
	while (shell->tokens->fls[index] != NULL)
	{
		file = open(shell->tokens->fls[index], O_RDONLY);
		if (file < 0)
		{
			close(infile);
			//* printf error ?//
			return (-1);
		}
		int count = 0;
		int	read_byte = 0;
		char *buff = (char *)ft_calloc(sizeof(char), 100000);
		// ? read_write_file()
		while (1)
		{
			read_byte = read(0, buff, 9999);
			buff[read_byte] = '\0';
			if (read_byte <= 0)
				break ;
			ft_putstr_fd(buff, fd);
		}
		close(file);
		index++;
	}

	dup2(infile, 0);
	close(infile);
	return (0);
}

int	execution_token(t_shell *shell, char *path, char **command)
{
	pid_t	pid;
	int 	fd[2];
	int		count = 0;
	int		outfile = 0;

	while (shell->tokens->fg[count] != NULL)
		count++;
	while (shell->tokens->fgg[outfile] != NULL)
		outfile++;
	count += outfile;
	// * make int file*/
	if (shell->tokens->fls != NULL || shell->tokens->flsls != NULL)
	{
		//* function make infile //
	}



	while(count > 0)
	{
		int pi = pipe(fd);
		printf("pi = %d\n", pi);
		pid = fork();
		if (pid < 0 )
			return (-1);
		else if (pid == 0)
		{
			signal_defualt();
			if (minishell_redirect(shell, fd, index) == -1)
				return (-1);
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
		count--;
	}
	return (status);
}
