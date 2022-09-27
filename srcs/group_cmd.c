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

t_cmd	*group_cmd(t_list *token)
{
	int		ctoken;
	t_list	*lst;

	lst = token;
	ctoken = count_cmds(token);
	if (ctoken < 0)
		printf("error token near syntax\n");
	printf("ctoken: %d\n", ctoken);
	// while (lst)
	// {
	// 	printf("t: %s\n", (char *)lst->content);
	// 	lst = lst->next;
	// }
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
		printf("t: %s\n", (char *) lst->content);
		opt = validate_opt(lst);
		if (opt == -1)
			return (-1);
		if (opt == 1)
			count++;
		lst = lst->next;
	}
	return (++count);
}