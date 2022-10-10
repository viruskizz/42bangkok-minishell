/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_util2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 17:25:00 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/11 00:25:21 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double_pointer(char **str1, char **str2, void *str3)
{
	int	index;

	index = 0;
	if (str1 != NULL)
	{
		while(str1[index] != NULL)
			free(str1[index++]);
		free(str1);
		str1 = NULL;
	}
	index = 0;
	if (str2 != NULL)
	{
		while(str2[index] != NULL)
			free(str2[index++]);
		free(str2);
		str2 = NULL;
	}
	if (str3 != NULL)
	{
		free(str3);
		str3 = NULL;
	}
}

char	**doublepointer_join_copy(char **result, char **strs, int *index)
{
	int	len;
	int	xedni;

	len = ft_lencount(NULL, strs, STRS);
	result = (char **)ft_calloc(sizeof(char *), len + 2);
	if (result == NULL)
		return (0);
	while (strs != NULL && strs[*index] != NULL)
	{
		len = ft_lencount(strs[*index], NULL, STR);
		result[*index] = (char *)ft_calloc(sizeof(char), len + 1);
		if (result == NULL)
		{
			free_double_pointer(result, NULL, NULL);
			return (0);
		}
		xedni = 0;
		while (strs[*index][xedni] != '\0')
		{
			result[*index][xedni] = strs[*index][xedni];
			xedni++;
		}
		(*index)++;
	}
	return (result);
}

char	**doublepointer_join(char **strs, char *str)
{
	char    **result;
	int     index;
	int     xedni;

	index = 0;
	result = doublepointer_join_copy(result, strs, &index);
	printf("index = %d, lencount = %d\n", index, ft_lencount(NULL, result, STRS));
	if (result == NULL)
		return(0);
	xedni = 0;
	result[index] = ft_calloc(sizeof(char), ft_lencount(str, NULL, STR) + 1);
	if (result == NULL)
	{
		free_double_pointer(result, NULL, NULL);
		return (0);
	}
	while (str != NULL && str[xedni] != '\0')
	{
		result[index][xedni] = str[xedni];
		xedni++;
	}
	result[++index] = NULL;
	free_double_pointer(strs, NULL, NULL);
	return(result);
}



// int main (void)
// {
//     char *strs[] = {"hello1", "hello2", "hello3", "hello4", NULL};
//     char **new;

//     for (int i = 0; strs[i] != NULL; i++)
//     {
//         printf("=> %s\n", strs[i]);
//     }

//     new = doublepointer_join(strs, "newnewnew");
//     printf("\n=======================\n\n");
//     for (int i = 0; new[i] != NULL; i++)
//     {
//         printf("=> %s\n", new[i]);
//     }
//     return (0);
// }