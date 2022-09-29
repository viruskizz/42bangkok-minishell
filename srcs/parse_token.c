/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiva <tsomsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 16:53:08 by araiva            #+#    #+#             */
/*   Updated: 2022/09/28 16:53:10 by araiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parse_dq_quote(t_list *token);
static void	parse_normal(t_list *lst);
static t_list	*parse_wildcard(t_list **lst);

t_list	*parse_token(t_list *tokens)
{
	t_list	*lst;

	lst = tokens;
	while (lst)
	{
		if (is_dq_str(lst->content))
			parse_dq_quote(lst);
		else if (!is_sq_str(lst->content))
			parse_normal(lst);
		if (ft_strchr(lst->content, '*'))
			lst = parse_wildcard(&lst);
		lst = lst->next;
	}
	return (tokens);
}

static void	parse_dq_quote(t_list *lst)
{
	char	*token;
	char	*s;
	char	*new;
	int		wlen;

	token = lst->content;
	token++;
	new = ft_strdup("\"");
	while (*token && *token != '"')
	{
		if (*token == '$')
			wlen = exp_env(token, &new);
		else
			wlen = exp_str(token, &new);
		token += wlen;
	}
	s = ft_strdup("\"");
	new = my_strcat(new, s);
	free(lst->content);
	lst->content = new;
}

static void	parse_normal(t_list *lst)
{
	char	*token;
	char	*s;
	char	*new;
	int		wlen;

	token = lst->content;
	new = ft_calloc(1, sizeof(char));
	while (*token && *token != '"')
	{
		if (*token == '~')
			wlen = exp_env_hom(token, &new);
		else if (*token == '$')
			wlen = exp_env(token, &new);
		else
			wlen = exp_str(token, &new);
		token += wlen;
	}
	free(lst->content);
	lst->content = new;
}

static t_list	*parse_wildcard(t_list **tokens)
{
	t_list	*paths;
	t_list	*next;

	paths = wild_paths(*tokens);
	next = (*tokens)->next;
	(*tokens)->content = paths->content;
	(*tokens)->next = paths->next;
	ft_lstadd_back(tokens, next);
	return (*tokens);
}
