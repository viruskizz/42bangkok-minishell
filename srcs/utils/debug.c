/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiva <tsomsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 19:04:36 by araiva            #+#    #+#             */
/*   Updated: 2022/09/25 19:04:36 by araiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_arr(char **str)
{
	int	i;

	i = 0;
	printf("[");
	while (*str)
	{
		printf("%s", *str);
		if (*(str + 1))
			printf(", ");
		str++;
	}
	printf("]\n");
}

void	print_lst(t_list *lst)
{
	printf("{");
	while (lst)
	{
		if (lst->content)
			printf("%s", (char *)lst->content);
		if (lst->next)
			printf(", ");
		lst = lst->next;
	}
	printf("}\n");
}

void	print_cmd_table(t_list *cmds)
{
	t_cmd	*cmd;

	printf("%s[\n%s", GREEN, RESET);
	while (cmds)
	{
		if (cmds->content)
		{
			cmd = (t_cmd *) cmds->content;
			printf("  {\n");
			printf("    tokens: ");
			print_arr(cmd->tokens);
			printf("    n: %d\n", cmd->n);
			printf("  },\n");
		}
		cmds = cmds->next;
	}
	printf("%s]\n%s", GREEN, RESET);
}