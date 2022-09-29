/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiva <tsomsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 21:33:25 by araiva            #+#    #+#             */
/*   Updated: 2022/09/27 21:33:26 by araiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_cmd(char **tokens, t_list **cmds);
static char	**set_cmd_tokens(t_list *lst, int ctoken);
static int	count_tokens(t_list *token);

t_list	*group_cmd(t_list *token)
{
	int		ctoken;
	int		i;
	t_list	*lst;
	t_list	*cmds;
	char	**tokens;

	lst = token;
	cmds = NULL;
	while (lst)
	{
		ctoken = count_tokens(lst);
		tokens = set_cmd_tokens(lst, ctoken);
		while (ctoken-- > 0)
			lst = lst->next;
		add_cmd(tokens, &cmds);
	}
	return (cmds);
}

static char	**set_cmd_tokens(t_list *lst, int ctoken)
{
	int		i;
	char	**tokens;

	tokens = ft_calloc(ctoken + 1, sizeof(char *));
	i = 0;
	while (i < ctoken)
	{
		tokens[i++] = ft_strdup((char *) lst->content);
		lst = lst->next;
	}
	tokens[i] = NULL;
	return (tokens);
}

static void	add_cmd(char **tokens, t_list **cmds)
{
	t_list	*new;
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	cmd->tokens = tokens;
	cmd->n = 0;
	new = ft_lstnew(cmd);
	if (!cmds)
		*cmds = new;
	else
		ft_lstadd_back(cmds, new);
}

static int	count_tokens(t_list *token)
{
	t_list	*lst;
	char	*t;
	int		i;

	lst = token;
	i = 0;
	while (lst)
	{
		t = (char *) lst->content;
		if (is_opt(t) == 1)
			return (++i);
		i++;
		lst = lst->next;
	}
	return (i);
}
