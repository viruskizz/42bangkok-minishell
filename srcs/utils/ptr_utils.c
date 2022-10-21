/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ptr_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 00:16:33 by araiva            #+#    #+#             */
/*   Updated: 2022/10/21 15:11:28 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	free_db_ptr(char **str1, char **str2, void *str3)
{
	int	index;

	index = 0;
	if (str1 != NULL)
	{
		while (str1[index] != NULL)
			free(str1[index++]);
		free(str1);
		str1 = NULL;
	}
	index = 0;
	if (str2 != NULL)
	{
		while (str2[index] != NULL)
			free(str2[index++]);
		free(str2);
		str2 = NULL;
	}
	if (str3 != NULL)
	{
		free(str3);
		str3 = NULL;
	}
	return (1);
}

char	*strtranfer_no_quote(char *str1, char *str2)
{
	int	index;
	int	xedni;
	int	len;

	len = 0;
	index = 0;
	xedni = 0;
	while (str2[index] != '\0')
	{
		if (str2[index] != '\'' && str2[index] != '\"')
			len++;
		index++;
	}
	str1 = ft_calloc(sizeof(char), len + 1);
	if (str1 == NULL)
		return (0);
	index = 0;
	while (str2[index] != '\0')
	{
		if (str2[index] != '\'' && str2[index] != '\"')
			str1[xedni++] = str2[index];
		index++;
	}
	str1[xedni] = '\0';
	return (str1);
}

char	*midjoin_free(char *str1, char *str2, char c)
{
	char	*result;

	result = ft_midjoin(str1, str2, c);
	if (result == NULL)
		return (0);
	if (str1 != NULL)
		free(str1);
	str1 = NULL;
	return (result);
}
