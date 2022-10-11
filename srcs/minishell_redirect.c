/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_redirect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 00:09:42 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/11 23:16:41 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*heredoc_convert_env(t_env *envs, char *buff, int index, int xedni)
{
	t_env	*env;
	int		count;
	char	*str;

	str = ft_strdup(buff);
	ft_bzero((void *)buff, (size_t)ft_lencount(str, NULL, STR));
	while (str[index] != '\0')
	{
		if (str[index] == '$')
		{
			env = convert_env_find(envs, str, &index);
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
	char	*line;
	t_env	*env;

	line = (char *)ft_calloc(sizeof(char), 10000);
	if (line == NULL)
		return (-1);
	while (true)
	{
		env = shell->env;
		line = readline("(heredoc)> ");
		if (line == NULL)
			break ;
		line[ft_lencount(line, NULL, STR)] = '\n';
		line[ft_lencount(line, NULL, STR) + 1] = '\n';
		if (string_compare(line, delimiter, NO_LEN) == 1)
			break ;
		heredoc_convert_env(shell->env, line, 0, 0);
		ft_putstr_fd(line, infile);
	}
	free(line);
	return (0);
}

int	redirect_read_heredoc(t_shell *shell, char **files, int infile)
{
	char	*delimiter;
	int		file;
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
