/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiva <tsomsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 13:37:35 by araiva            #+#    #+#             */
/*   Updated: 2022/09/24 13:37:36 by araiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	wordlen(char *str);
static int	count_word(char *line);


char	**split_input(char *line)
{
	int	i;
	int	wlen;
	char **words;

	i = 0;
	words = ft_calloc(sizeof(char *), count_word(line) + 1);
	while (*line)
	{
		wlen = wordlen(line);
		words[i] = ft_calloc(sizeof(char), wlen + 1);
		ft_strlcpy(words[i++], line, wlen + 1);
		line += wlen;
		while (is_spt(*(++line)))
			if (!*line)
				break;
	}
	words[i] = NULL;
	return (words);
}

static int	wordlen(char *str)
{
	int	i;

	i = 0;
	if (is_end_quote(str[i]))
	{
		while (str[++i])
		{
			if(is_end_quote(str[i]))
				return (i);
		}
		ft_putstr_fd("Unexpected token quoting\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	while (str[i] && !is_spt(str[i]))
		i++;
	return (i);
}

static int	count_word(char *s)
{
	int	count;
	int wlen;

	count = 0;
	while (*s)
	{
		wlen = wordlen(s);
		s = s + wlen;
		if (wlen > 0)
			count++;
		while (is_spt(*(++s)))
			if (!*s)
				break;
	}
	return (count);
}