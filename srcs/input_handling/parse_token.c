/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 16:53:08 by araiva            #+#    #+#             */
/*   Updated: 2022/10/13 22:52:33 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char		*parser(char *str, t_shell *shell);
static int		parse_dq_quote(char *str, char **new, t_shell *shell);
static int		parse_normal(char *str, char **new, t_shell *shell);
static t_list	*parse_wildcard(t_list **lst);

t_list	*parse_token(t_list *tokens, t_shell *shell)
{
	t_list	*lst;
	char	*new;

	lst = tokens;
	while (lst)
	{
		new = parser(lst->content, shell);
		free(lst->content);
		lst->content = new;
		if (ft_strchr(lst->content, '*'))
			lst = parse_wildcard(&lst);
		lst = lst->next;
	}
	return (tokens);
}

static char	*parser(char *str, t_shell *shell)
{
	char	*new;
	int		wlen;

	new = ft_calloc(1, sizeof(char));
	if (*str == '~' && !ft_strchr(str, '"') && !ft_strchr(str, '\''))
		str += exp_env_hom(str, &new, shell);
	while (*str)
	{
		if (*str == '\'')
			wlen = exp_sq_str(str, &new);
		else if (*str == '\"')
			wlen = parse_dq_quote(str, &new, shell);
		else
			wlen = parse_normal(str, &new, shell);
		str += wlen;
	}
	return (new);
}

static int	parse_normal(char *str, char **new, t_shell *shell)
{
	int	wlen;

	if (ft_strchr(QUOTES, *str))
		return (0);
	if (*str == '$')
		wlen = exp_env(str, new, shell);
	else
		wlen = exp_str(str, new);
	return (wlen);
}

static int	parse_dq_quote(char *str, char **new, t_shell *shell)
{
	int	wlen;
	int	len;

	wlen = 1;
	if (*str != '"')
		return (0);
	str++;
	while (*str && *str != '"')
	{
		if (*str == '$')
			len = exp_env(str, new, shell);
		else
			len = exp_dq_str(str, new);
		str += len;
		wlen += len;
	}
	return (wlen + 1);
}

static t_list	*parse_wildcard(t_list **tokens)
{
	t_list	*paths;
	t_list	*next;

	paths = wild_paths(*tokens);
	if (paths)
	{
		next = (*tokens)->next;
		(*tokens)->content = paths->content;
		(*tokens)->next = paths->next;
		ft_lstadd_back(tokens, next);
	}
	return (*tokens);
}
