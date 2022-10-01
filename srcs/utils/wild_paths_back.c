/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_paths.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiva <tsomsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 04:43:27 by araiva            #+#    #+#             */
/*   Updated: 2022/10/01 04:43:28 by araiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*wild_paths(t_list *tokens)
{
	t_list			*paths;
	t_list			*new;
	char			*dirname;
	DIR				*dir;
	struct dirent	*entry;
	char			*basedir;
	char			*srch;

	paths = NULL;
	srch = tokens->content;
	if (*srch != '/')
		basedir = ".";
	if ((dir = opendir("srcs")) == NULL)
		perror("opendir() error");
	else
	{
		while ((entry = readdir(dir)) != NULL)
		{
			dirname = ft_strdup(entry->d_name);
			new = ft_lstnew(dirname);
			if (!paths)
				paths = new;
			else
				ft_lstadd_back(&paths, new);
		}
		closedir(dir);
	}
	return (paths);
}

// int	str_wildcards(char *str, char *srch)
// {
// 	while (*srch)
// 	{
// 		if (*srch == '*' && *srch++)
// 		{
// 			while (*str && *srch != *(++str));
// 			if (!*srch)
// 				return (1);
// 		}
// 		if (*srch != *str)
// 			return (0);
// 		str++;
// 		srch++;
// 	}
// 	if (!*srch)
// 		return (1);
// 	else
// 		return (0);
// }

// int main()
// {
// 	char *str = "aaabbccdef";
// 	printf("search from str: %s\n", str);
// 	printf("a*	> %d\n", str_wildcards(str, "a*"));
// 	printf("ab*	> %d\n", str_wildcards(str, "ab*"));
// 	printf("*f	> %d\n", str_wildcards(str, "*f"));
// 	printf("a*f	> %d\n", str_wildcards(str, "*f"));
// 	printf("a*d*f	> %d\n", str_wildcards(str, "*f"));
// 	printf("a*d*ef	> %d\n", str_wildcards(str, "*f"));
// 	printf("a*d*xf*	> %d\n", str_wildcards(str, "*f"));
// 	printf("a*d*	> %d\n", str_wildcards(str, "*f"));
// 	return (0);
// }
