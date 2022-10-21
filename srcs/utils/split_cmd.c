/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 01:51:03 by sharnvon          #+#    #+#             */
/*   Updated: 2022/09/26 21:41:27 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	split_count(char *str, char c, int mode)
{
	int	result;
	int	index;

	result = 0;
	index = 0;
	if (mode == WORD)
	{
		while (str[index] != '\0')
		{
			if (str[index] != c && str[index] != '\0')
				result += 1;
			while (str[index] != c && str[index] != '\0')
				index++;
			while (str[index] == c && str[index] != '\0')
				index++;
		}
	}
	if (mode == LETTER)
	{
		while (str[result] != '\0' && str[result] != c)
			result++;
	}
	return (result);
}

char	**split_wordmaker(char **result, char *str, char c)
{
	int	index;
	int	xedni;
	int	len;

	index = 0;
	xedni = 0;
	while (str[index] != '\0')
	{
		while (str[index] == c)
			index++;
		len = split_count(&str[index], c, LETTER);
		result[xedni] = (char *)ft_calloc(1, len + 1);
		if (result[xedni++] == NULL)
		{
			while (xedni - 1 >= 0)
				free(result[xedni--]);
			return (0);
		}
		index += len;
		while (str[index] == c)
			index++;
	}
	result[xedni] = NULL;
	return (result);
}

char	**split_strtranfer(char **result, char *str, char c)
{
	int	index;
	int	xedni;
	int	word;

	index = 0;
	word = 0;
	while (str[index] != '\0')
	{
		while (str[index] == c)
			index++;
		xedni = 0;
		while (str[index] != '\0' && str[index] != c)
			result[word][xedni++] = str[index++];
		result[word][xedni] = '\0';
		word++;
		while (str[index] == c)
			index++;
	}
	result[word] = NULL;
	return (result);
}

// function split string str by c charactor with mode//
// (mode) FREE: free string str | BOUND: keep string str //:
char	**ft_split_mode(char *str, char c, int mode)
{
	char	**result;

	result = (char **)ft_calloc(sizeof(char *), split_count(str, c, WORD) + 1);
	if (result == NULL)
		return (0);
	result = split_wordmaker(result, str, c);
	if (result == NULL)
		return (0);
	result = split_strtranfer(result, str, c);
	if (mode == FREE)
		free(str);
	return (result);
}
