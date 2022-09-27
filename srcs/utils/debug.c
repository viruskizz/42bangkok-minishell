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
	ft_putstr_fd("[", STDOUT_FILENO);
	while (*str)
	{
		ft_putstr_fd(*str, STDOUT_FILENO);
		if (*(str + 1))
			ft_putstr_fd(", ", STDOUT_FILENO);
		str++;
	}
	ft_putstr_fd("]\n", STDOUT_FILENO);
}

void	print_lst(t_list *lst)
{
	ft_putstr_fd("{", STDOUT_FILENO);
	while (lst)
	{
		if (lst->content)
			ft_putstr_fd((char *)lst->content, STDOUT_FILENO);
		if (lst->next)
			ft_putstr_fd(", ", STDOUT_FILENO);
		lst = lst->next;
	}
	ft_putstr_fd("}\n", STDOUT_FILENO);
}
