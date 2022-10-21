/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 04:41:45 by araiva            #+#    #+#             */
/*   Updated: 2022/10/07 21:18:35 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**lst_to_arr(t_list *lst)
{
	int		i;
	int		n;
	t_list	*tmp;
	char	**arr;

	i = 0;
	n = ft_lstsize(lst);
	if (n == 0)
		return (0);
	arr = ft_calloc(n + 1, sizeof(char *));
	while (i < n)
	{
		arr[i++] = ft_strdup(lst->content);
		tmp = lst;
		lst = lst->next;
		ft_lstdelone(tmp, &free_token);
	}
	arr[i] = NULL;
	return (arr);
}

int	arr_str_len(char **arr)
{
	int		i;

	i = 0;
	while (arr[++i])
		;
	return (i);
}

void	free_arr(char **arr)
{
	int		i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr[i]);
	free(arr);
}
