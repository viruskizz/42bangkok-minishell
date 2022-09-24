/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_util1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 22:48:33 by sharnvon          #+#    #+#             */
/*   Updated: 2022/09/24 14:08:38 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	*ft_calloc(int count, int size)
// {
// 	void	*result;
// 	int		index;

// 	index = 0;
// 	result = malloc(count * size);
// 	if (result == NULL)
// 		return (0);
// 	while (index < (count * size))
// 	{
// 		((char *)result)[index] = '\0';
// 		index++;
// 	}
// 	return (result);
// }
// function compare two strings //
	//return:1 strings are same | return:0 string aren't same //
int	string_compare(char *str1, char *str2)
{
	int	index;

	index = 0;
	while (str1[index] == str2[index])
	{
		if (str1[index] == '\0' && str2[index] == '\0')
			return (1);
		index++;
	}
	return (0);
}

// function search character in string str //
// (mode) FRONT: find from front  | BACK: find from back //
int	character_search(char *str, char c, int mode)
{
	int	index;

	if (mode == FRONT)
	{
		index = 0;
		while (str[index] != '\0')
		{
			if (str[index] == c)
				return (1);
			index++;
		}
	}
	else if (mode == BACK)
	{
		index = ft_lencount(str, NULL, STR) - 1;
		while (index > -1)
		{
			if (str[index] == c)
				return (1);
			index--;
		}
	}
	return (0);
}

// function count len of char* and char** //
	// (mode) STR: count len of char * | STRS count len of char ** //

int	ft_lencount(char *str, char **strs, int mode)
{
	int	count;

	count = 0;
	if (mode == STR)
	{
		while (str[count] != '\0')
			count++;
	}
	else if (mode == STRS)
	{
		while (strs[count] != NULL)
			count++;
	}
	return (count);
}

// function join string and add charactor c in the middle //
// example: "1111" + "aa" + '/' = "1111/aa"
char	*ft_midjoin(char *str1, char *str2, char c)
{
	char	*result;
	int		index;
	int		xedni;

	index = 0;
	xedni = 0;
	if (c == '\0')
		result = (char *)ft_calloc(sizeof(char), ft_lencount(str1, NULL, STR) + ft_lencount(str2, NULL, STR) + 1);
	else
		result = (char *)ft_calloc(sizeof(char), ft_lencount(str1, NULL, STR) + ft_lencount(str2, NULL, STR) + 2);
	if (result == NULL)
		return (0);
	while (str1[index] != '\0')
	{
		result[index] = str1[index];
		index++;
	}
	if (c != '\0')
		result[index++] = c;
	while (str2[xedni] != '\0')
		result[index++] = str2[xedni++];
	result[index] = '\0';
	return (result);
}
