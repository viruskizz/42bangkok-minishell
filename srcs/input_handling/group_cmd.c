/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 21:33:25 by araiva            #+#    #+#             */
/*   Updated: 2022/10/07 21:08:22 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*lst_rd_new(char *symbol, t_list *lst, int ctoken);
static int		set_cmd(t_list *lst, t_cmd **cmd);
static int		count_tokens(t_list *token);

t_list	*group_cmd(t_list *lst)
{
	t_list	*cmds;
	t_cmd	*cmd;
	int		ctoken;

	cmds = NULL;
	while (lst)
	{
		ctoken = set_cmd(lst, &cmd);
		if (!ctoken)
			break ;
		ft_lstadd_back(&cmds, ft_lstnew(cmd));
		while (lst && ctoken-- > 0)
			lst = lst->next;
	}
	return (cmds);
}

static int	set_cmd(t_list *lst, t_cmd **cmd)
{
	int		i;
	int		n;
	int		opt;
	t_cmd	*new;

	i = 0;
	n = count_tokens(lst);
	new = ft_calloc(sizeof(t_cmd), 1);
	new->fg = lst_to_arr(lst_rd_new(">", lst, n));
	new->fgg = lst_to_arr(lst_rd_new(">>", lst, n));
	new->fls = lst_to_arr(lst_rd_new("<", lst, n));
	new->flsls = lst_to_arr(lst_rd_new("<<", lst, n));
	new->tokens = lst_to_arr(lst_rd_new(NULL, lst, n));
	opt = 0;
	while (i++ < n)
	{
		if (is_opt(lst->content))
			opt = parse_opt(lst->content);
		lst = lst->next;
	}
	new->opt = opt;
	*cmd = new;
	return (n);
}

static t_list	*lst_rd_new(char *smb, t_list *lst, int n)
{
	t_list	*dest;
	t_list	*new;
	char	*str;

	dest = NULL;
	while (n > 0)
	{
		str = lst->content;
		if (smb && !ft_strcmp(lst->content, smb) && n--)
		{
			lst = lst->next;
			str = lst->content;
			new = ft_lstnew(ft_strdup(str));
			ft_lstadd_back(&dest, new);
		}
		else if (smb == NULL && is_redirect(str) && !is_opt(str) && n--)
			lst = lst->next;
		else if (smb == NULL && !is_redirect(str) && !is_opt(str))
			ft_lstadd_back(&dest, ft_lstnew(ft_strdup(str)));
		n--;
		lst = lst->next;
	}
	return (dest);
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
		if (is_opt(t) > 0)
			return (++i);
		i++;
		lst = lst->next;
	}
	return (i);
}

void	free_cmd(void *content)
{
	t_cmd	*cmd;

	cmd = (t_cmd *) content;
	free_arr(cmd->tokens);
	free_arr(cmd->fg);
	free_arr(cmd->fgg);
	free_arr(cmd->fls);
	free_arr(cmd->flsls);
	free(cmd);
}
