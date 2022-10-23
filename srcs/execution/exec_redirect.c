/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 00:09:42 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/13 01:34:24 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_inputpassing(t_shell *shell, int infile, char *delimiter)
{
	char	*line;
	char	*input;

	line = (char *)ft_calloc(sizeof(char), 10000);
	if (line == NULL)
		return (-1);
	while (true)
	{
		input = readline("(heredoc)> ");
		if (input == NULL)
			break ;
		string_tranfer(line, input, NO_LEN);
		line[ft_lencount(line, NULL, STR) + 1] = '\0';
		line[ft_lencount(line, NULL, STR)] = '\n';
		if (string_compare(line, delimiter, NO_LEN) == 1)
			break ;
		heredoc_convert_env(shell, line, 0, 0);
		ft_putstr_fd(line, infile);
	}
	free(line);
	return (0);
}

int	redirect_read_heredoc(t_shell *shell, char **files, int infile)
{
	char	*delimiter;
	int		index;

	index = 0;
	if (files == NULL)
		return (0);
	while (files[index] != NULL)
	{
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
	free(delimiter);
	return (0);
}

int	redirect_read_file(char **files, int infile, int index, int file)
{
	char	*buff;
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
		while (1 > 0)
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

	if (cmds->fls == NULL && cmds->flsls == NULL)
		return (0);
	infile = open(IN_FILE, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (infile < 0)
	{
		perror("minishell");
		return (-1);
	}
	if (redirect_read_file(cmds->fls, infile, 0, 0) == -1)
	{
		perror("minishell");
		close(infile);
		return (-1);
	}
	if (redirect_read_heredoc(shell, cmds->flsls, infile) == -1)
	{
		perror("minishell");
		close(infile);
		return (-1);
	}
	close(infile);
	return (0);
}
