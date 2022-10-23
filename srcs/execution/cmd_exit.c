/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 00:11:17 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/23 20:24:11 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exit_validation(t_shell *shell);
static void	exit_clear(t_shell *shell);

int	execution_exit(t_shell *shell, char **cmds)
{
	unsigned char	return_state;

	return_state = 0;
	exit_validation(shell);
	if (ft_lencount(NULL, cmds, STRS) == 1)
	{
		exit_clear(shell);
		printf("exit\n");
		exit(0);
	}
	else if (ft_lencount(NULL, cmds, STRS) == 2)
	{
		return_state = (unsigned char)ft_atoi(cmds[1]);
		exit_clear(shell);
		printf("exit\n");
		exit((int)return_state);
	}
	else
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	return (1);
}

static int	exit_validation(t_shell *shell)
{
	int		index;
	int		xedni;
	t_cmd	*cmds;

	cmds = (t_cmd *)shell->cmds->content;
	index = 1;
	while (cmds->tokens[index] != NULL)
	{
		xedni = 0;
		if (cmds->tokens[index][xedni] == '-')
			xedni++;
		while (cmds->tokens[index][xedni] != '\0')
		{
			if (ft_isdigit(cmds->tokens[index][xedni++]) == 0)
			{
				ft_putstr_fd("exit\nminishell: exit: ", 2);
				ft_putstr_fd(cmds->tokens[index], 2);
				ft_putstr_fd(": numeric argument required\n", 2);
				exit_clear(shell);
				exit(255);
			}
		}
		index++;
	}
	return (1);
}

static void	exit_clear(t_shell *shell)
{
	ft_lstclear(&shell->cmds, &free_cmd);
	rl_clear_history();
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->terminal->shell);
	free_db_ptr(NULL, NULL, shell->terminal);
	environment_clear(&shell->env);
}
