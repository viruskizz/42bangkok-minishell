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

static int	set_cmd(t_list *lst, t_cmd **cmd);

static char	**set_cmd_tokens(t_list *lst, int ctoken);
static int	count_tokens(t_list *token);

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
		if (!cmds)
			cmds = ft_lstnew(cmd);
		else
			ft_lstadd_back(&cmds, ft_lstnew(cmd));
		while (ctoken-- > 0)
			lst = lst->next;
	}
	return (cmds);
}
// aa bb cc << ii cc >> jjj || x y z
static int	set_cmd(t_list *lst, t_cmd **cmd)
{
	int		i;
	int		ctoken;
	char	*str;
	t_cmd	*new;
	t_list	*tmp;
	t_list	*tokens;
	t_list	*inputs;
	t_list	*outputs;
	int		opt;

	ctoken = count_tokens(lst);
	printf("ctoken: %d\n", ctoken);
	i = 0;
	opt = 0;
	tokens = NULL;
	inputs = NULL;
	outputs = NULL;
	while (i < ctoken)
	{
		str = lst->content;
		// printf("%d: %s\n", i, str);
		if (is_opt(str))
		{
			// printf("opt\n");
			opt = parse_opt(str);
		}
		else if (!ft_strcmp(str, ">>"))
		{
			// printf("out\n");
			i++;
			lst = lst->next;
			tmp = ft_lstnew(ft_strdup(lst->content));
			ft_lstadd_back(&outputs, tmp);
		}
		else if (!ft_strcmp(str, "<<"))
		{
			// printf("in\n");
			i++;
			lst = lst->next;
			tmp = ft_lstnew(ft_strdup(lst->content));
			ft_lstadd_back(&inputs, tmp);
		}
		else
		{
			tmp = ft_lstnew(ft_strdup(str));
			ft_lstadd_back(&tokens, tmp);
		}
		i++;
		lst = lst->next;
	}
	new = malloc(sizeof(t_cmd));
	printf("tokens:");
	print_lst(tokens);
	printf("inputs:");
	print_lst(inputs);
	printf("outputs:");
	print_lst(outputs);
	printf("opt: %d\n", opt);
	*cmd = new;
	return (ctoken);
}


// static char	**set_cmd_tokens(t_list *lst, int ctoken)
// {
// 	int		i;
// 	char	**tokens;

// 	tokens = ft_calloc(ctoken + 1, sizeof(char *));
// 	i = 0;
// 	while (i < ctoken)
// 	{
// 		tokens[i++] = ft_strdup((char *) lst->content);
// 		lst = lst->next;
// 	}
// 	tokens[i] = NULL;
// 	return (tokens);
// }

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
