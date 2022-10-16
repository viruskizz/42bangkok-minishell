/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_paths.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 04:43:27 by araiva            #+#    #+#             */
/*   Updated: 2022/10/13 22:40:45 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_match_path(char *srch, char *pattern);
static void	*get_paths(char *dirname, char *srch, t_list **paths);
static int	is_end_path(char *str);

t_list	*wild_paths(t_list *tokens)
{
	t_list	*paths;
	char	*srch;

	paths = NULL;
	srch = tokens->content;
	if (*srch == '.' && *(srch + 1) == '/')
		srch += 2;
	if (*srch == '/' && srch++)
		get_paths("/", srch, &paths);
	else
		get_paths(".", srch, &paths);
	print_lst(paths);
	return (paths);
}

static void	*get_paths(char *dirname, char *srch, t_list **paths)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*str;
	int				is_mathch;

	dir = opendir(dirname);
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (entry)
	{
		str = entry->d_name;
		is_mathch = is_match_path(str, srch);
		if (is_mathch > 0)
			get_paths(str, srch + is_mathch + 1, paths);
		else if (is_mathch == 0)
			ft_lstadd_back(paths, ft_lstnew(ft_strdup(str)));
		entry = readdir(dir);
	}
	closedir(dir);
	return (NULL);
}

/**
 * @brief 
 * the mean of return value
 * 0 is end of pattern string;
 * -1 is not found search string fro
 * @param str 
 * @param srch 
 * @return int 
 */
static int	is_match_path(char *str, char *srch)
{
	int	i;

	i = 0;
	while (*srch)
	{
		if (*srch == '*' && *srch++ && ++i)
		{
			while (*str && *srch != *(++str))
				;
			if (is_end_path(srch))
				return (0);
		}
		if (*srch == '/')
			return (i);
		if (*srch != *str)
			return (-1);
		i++;
		str++;
		srch++;
	}
	if (!*srch)
		return (0);
	else
		return (-100);
}

static	int	is_end_path(char *str)
{
	if (!*str)
		return (1);
	if (*str == '/' && !*(str + 1))
		return (1);
	return (0);
}
