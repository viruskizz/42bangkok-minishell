/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 18:52:43 by araiva            #+#    #+#             */
/*   Updated: 2022/09/29 22:22:13 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wordlen(char *str);
static int	add_token(t_list **token, char *line);

t_list	*split_input(char *line)
{
	int		wlen;
	t_list	*token;

	token = NULL;
	while (*line)
	{
		wlen = add_token(&token, line);
		if (wlen < 0)
		{
			ft_lstclear(&token, &free_token);
			return (NULL);
		}
		line += wlen;
		if (!ft_strchr(FIELDS, *line))
			continue ;
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
	if (wlen < 0)
		return (-1);
	word = ft_calloc(sizeof(char), wlen + 1);
	ft_strlcpy(word, line, wlen + 1);
	new = ft_lstnew(word);
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
	if (is_opt(str) > 0)
		return (is_opt(str));
	else if (is_redirect(str) > 0)
		return (is_redirect(str));
	while (str[i] && !ft_strchr(FIELDS, str[i]))
		i++;
	return (i);
}

void	free_token(void *content)
{
	free(content);
}
