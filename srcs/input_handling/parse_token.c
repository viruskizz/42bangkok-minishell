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

static void		parse_dq_quote(t_list *token, t_shell *shell);
static void		parse_normal(t_list *lst, t_shell *shell);
static t_list	*parse_wildcard(t_list **lst);
static int		quote_remove(t_list *token);

t_list	*parse_token(t_list *tokens, t_shell *shell)
{
	t_list	*lst;

	lst = tokens;
	while (lst)
	{
		if (is_dq_str(lst->content))
			parse_dq_quote(lst, shell);
		else if (!is_sq_str(lst->content))
			parse_normal(lst, shell);
		if (!is_dq_str(lst->content)
			&& !is_sq_str(lst->content)
			&& ft_strchr(lst->content, '*'))
			lst = parse_wildcard(&lst);
		quote_remove(lst);
		lst = lst->next;
	}
	return (tokens);
}

static void	parse_dq_quote(t_list *lst, t_shell *shell)
{
	char	*token;
	char	*new;
	int		wlen;

	token = lst->content;
	new = ft_strdup("\"");
	token++;
	while (*token)
	{
		if (*token == '$')
			wlen = exp_env(token, &new, shell);
		else
			wlen = exp_str(token, &new);
		token += wlen;
	}
	free(lst->content);
	lst->content = new;
}

static void	parse_normal(t_list *lst, t_shell *shell)
{
	char	*token;
	char	*new;
	int		wlen;

	token = lst->content;
	new = ft_calloc(1, sizeof(char));
	while (*token && *token != '"')
	{
		if (*token == '~')
			wlen = exp_env_hom(token, &new, shell);
		else if (*token == '$')
			wlen = exp_env(token, &new, shell);
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
	if (paths)
	{
		next = (*tokens)->next;
		(*tokens)->content = paths->content;
		(*tokens)->next = paths->next;
		ft_lstadd_back(tokens, next);
	}
	return (*tokens);
}

static int	strcpy_quote(char *src, char *dest);

static	int	quote_remove(t_list *token)
{
	char	*s;
	char	*new;
	int		i;
	int		j;

	s = token->content;
	i = 0;
	j = 0;
	new = ft_calloc(ft_strlen(s), sizeof(char));
	if (is_dq_str(s) || is_sq_str(s))
	{
		new = ft_calloc(ft_strlen(s), sizeof(char));
		while (s[i])
		{
			if (s[i] != s[0])
				new[j++] = s[i];
			i++;
		}
		token->content = new;
		free(s);
	}
	// while (*s)
	// {
	// 	if (!ft_strchr(QUOTES, *s))
	// 		new[i++] = *s++;
	// 	else if (is_dq_str(s) < -1 || is_sq_str(s) < -1)
	// 	{
	// 		free(new);
	// 		return (0);
	// 	}
	// 	else
	// 	{
	// 		i += strcpy_quote(s, &new[i]);
	// 		s += i;
	// 	}
	// }
	// new[i] = 0;
	// s = token->content;
	// token->content = new;
	// free(s);
	return (1);
}

static int	strcpy_quote(char *src, char *dest)
{
	int		i;
	int		j;

	i = 1;
	j = 0;
	printf("strcpy_quote\n");
	if (!is_dq_str(src) && !is_sq_str(src))
		return (0);
	while (src[i])
	{
		if (src[i] != src[0])
			dest[j++] = src[i];
		i++;
	}
	printf("dest: %s\n", dest);
	return (i);
}