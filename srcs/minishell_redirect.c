/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_redirect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 00:09:42 by sharnvon          #+#    #+#             */
/*   Updated: 2022/09/26 02:17:44 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int heredoc_inputpassing(char *buff, char *delimiter)
{
    int read_byte;
    int fd;

    fd = open(HERE_DOC, O_RDWR | O_CREAT | O_APPEND, 0777);
    if (fd < 0)
        return (-1);
    while (1)
    {
        read_byte = read(0, buff, 9999);
        buff[read_byte] = '\0';
        if (string_compare(buff, delimiter) == 1)
            break ;
        // need to convert the env variable //
        ft_putstr_fd(buff, fd);
    }
    close(fd);
    fd = open(HERE_DOC, O_RDWR | O_CREAT | O_APPEND, 0777);
    if (fd < 0)
        return (-1);
    return (fd);
}

int redirect_from_heredoc(t_shell *shell, int index)
{
    char	*buff;
    char	*delimiter;
	int		fd;

    delimiter = ft_midjoin(shell->tokens[index].file, "\n", '\0');
    if (delimiter == NULL)
        return (-1);
    buff = (char *)ft_calloc(sizeof(char), 10000);
    if (buff == NULL)
    {
        free(delimiter);
        return (-1);
    }
    fd = heredoc_inputpassing(buff, delimiter);
    if (fd < 0)
    {
		free(delimiter);
		free(buff);
		return(-1);
    }
	dup2(fd, 0);
	close(fd);
	free(buff);
	free(delimiter);
	return (0);
}

int redirect_from_file(t_shell *shell, int index)
{
	int	fd;

	fd = open(shell->tokens[index].file, O_RDWR);
	if (fd < 0)
		return (-1);
	dup2(fd, 0);
	close(fd);
	return (0);
}

int redirect_to_file(t_shell *shell, int index)
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
	if (shell->tokens[index].redir == FROM)
	{
		if (redirect_from_file(shell, index) == -1)
			return (-1);
	}
	else if (shell->tokens[index].redir == FFROM)
	{
		if (redirect_from_heredoc(shell, index) == -1)
			return (-1);
	}
	if (shell->tokens[index].redir == TO || shell->tokens[index].redir == TTO)
	{
		if (redirect_to_file(shell, index) == -1)
			return (-1);
	}
	else if (index < shell->cmd_amount - 1)
		dup2(fd[1], 1);
	return (0);
}