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

static int	count_cmds(t_list *token);
static int	count_tokens(t_list *token);

t_cmd	*group_cmd(t_list *token)
{
	int		ctoken;
	int		i;
	t_list	*lst;
	t_cmd	*cmds;
	t_cmd	*cmd;
	char	**tokens;

	if (count_cmds(token) < 0)
	{
		printf("error token near syntax\n");
		return (NULL);
	}
	lst = token;
	cmds = NULL;
	while (lst)
	{
		ctoken = count_tokens(lst);
		printf("ctoken: %d\n", ctoken);
		tokens = ft_calloc(ctoken + 1, sizeof(char *));
		i = 0;
		while (i++ < ctoken)
		{
			tokens[i] = ft_strdup((char *) lst->content);
			lst = lst->next;
		}
		tokens[i] = NULL;
	}
	return (NULL);
}

static int	count_cmds(t_list *token)
{
	t_list	*lst;
	int		opt;
	int		count;

	lst = token;
	count = 0;
	while (lst)
	{
		opt = validate_opt(lst);
		if (opt == -1)
			return (-1);
		if (opt == 1)
			count++;
		lst = lst->next;
	}
	return (++count);
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