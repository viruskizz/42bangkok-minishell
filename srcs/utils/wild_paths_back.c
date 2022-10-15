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
