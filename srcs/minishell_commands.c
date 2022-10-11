/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_commands.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 23:33:02 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/12 01:03:37 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execution_change_directory(t_shell *shell, char **command)
{
	char	*user;

	user = ft_midjoin("~", getenv("USER"), '\0');
	if (ft_lencount(NULL, command, STRS) == 3)
	{
		printf("cd: string not in pwd: %s\n", command[1]);
		shell->exstat = 1;
	}
	else if (ft_lencount(NULL, command, STRS) == 1 || string_compare(command[1], "~", NO_LEN) == 1 || string_compare(command[1], user, NO_LEN) == 1)
		shell->exstat = chdir(getenv("HOME"));
	else if (string_compare(command[1], "-", NO_LEN) == 1)
		shell->exstat = chdir(getenv("OLDPWD"));
	else if (ft_lencount(NULL, command, STRS) == 2)
		shell->exstat = chdir(command[1]);
	else
	{
		printf("cd: too many agurments");
		shell->exstat = 1;
	}
	free(user);
	return (0);
}