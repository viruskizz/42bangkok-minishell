/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiva <tsomsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 18:52:43 by araiva            #+#    #+#             */
/*   Updated: 2022/09/25 18:52:44 by araiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wordlen(char *str);
static int	count_word(char *line);
static int	add_token(t_list **token, char *line);

t_list	*split_input(char *line)
{
	int		wlen;
	t_list	*token;

	if (count_word(line) < 0)
		return (NULL);
	token = NULL;
	while (*line)
	{
		wlen = add_token(&token, line);
		line += wlen;
		while (*line && ft_strchr(FIELDS, *(++line)))
			if (!*line)
				break ;
	}
	return (token);
}

static int	add_token(t_list **token, char *line)
{
	int		wlen;
	char	*word;
	t_list	*new;

	wlen = wordlen(line);
	word = ft_calloc(sizeof(char), wlen + 1);
	ft_strlcpy(word, line, wlen + 1);
	new = ft_lstnew(word);
	line += wlen;
	if (!token)
		*token = new;
	else
		ft_lstadd_back(token, new);
	return (wlen);
}

static int	wordlen(char *str)
{
	int	i;

	i = 0;
	if (ft_strchr(QUOTES, str[0]))
	{
		while (str[++i])
			if (ft_strchr(QUOTES, str[i]))
				return (++i);
		perror("token reconize error quoting");
		return (-1);
	}
	while (str[i] && !ft_strchr(FIELDS, str[i]))
		i++;
	return (i);
}

static int	count_word(char *s)
{
	int	count;
	int	wlen;

	count = 0;
	while (*s)
	{
		wlen = wordlen(s);
		if (wlen < 0)
			return (-1);
		s = s + wlen;
		if (wlen > 0)
			count++;
		while (*s && ft_strchr(FIELDS, *(++s)))
			if (!*s)
				break ;
	}
	return (count);
}


void	free_token(void *content)
{
	free(content);
}
