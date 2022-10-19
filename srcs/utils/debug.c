/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 19:04:36 by araiva            #+#    #+#             */
/*   Updated: 2022/10/07 21:27:07 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//* @file debug.c
//* @author Araiva (viruskizz@github.io)
//* @brief debug the data in minishell
//* the symbol of result mean as below
//* < linklist > : Linklist is in start and end with triangle bracket is 
//* [ array ] : Array is in start and end with triangle bracket is 
//* { struct } : Struct is in start and end with triangle bracket is 
//* @version 0.1
//* @date 2022-10-01
//* @copyright Copyright (c) 2022

#include "minishell.h"

void	print_arr(char **str)
{
	printf("[ ");
	if (str == NULL)
		printf("NULL");
	while (str != NULL && *str)
	{
		printf("%s", *str);
		if (*(str + 1))
			printf(", ");
		str++;
	}
	printf(" ]\n");
}

void	print_lst(t_list *lst)
{
	printf("< ");
	while (lst)
	{
		if (lst->content)
			printf("%s", (char *) lst->content);
		if (lst->next)
			printf(", ");
		lst = lst->next;
	}
	printf(" >\n");
}

// void	print_cmd_table(t_list *cmds)
// {
// 	t_cmd	*cmd;

// 	printf("<\n");
// 	while (cmds)
// 	{
// 		if (cmds->content)
// 		{
// 			cmd = (t_cmd *) cmds->content;
// 			printf("  {\n");
// 			printf("    n: %d\n", arr_str_len(cmd->tokens));
// 			printf("    tokens: ");
// 			print_arr(cmd->tokens);
// 			printf("    fgt: ");
// 			print_arr(cmd->fg);
// 			printf("    fgtgt: ");
// 			print_arr(cmd->fgg);
// 			printf("    fls: ");
// 			print_arr(cmd->fls);
// 			printf("    flsls: ");
// 			print_arr(cmd->flsls);
// 			printf("    opt[%d]: %s\n", cmd->opt, str_opt(cmd->opt));
// 			printf("  },\n");
// 		}
// 		cmds = cmds->next;
// 	}
// 	printf(">\n");
// }
