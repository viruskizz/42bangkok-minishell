/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_signal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiva <tsomsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/16 23:08:25 by araiva            #+#    #+#             */
/*   Updated: 2022/10/16 23:08:27 by araiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_signal_handler(int signum);

void	execution_signal(t_shell *shell, int mode)
{
	if (mode == CHILD)
	{
		sigemptyset(&shell->sigint.sa_mask);
		shell->sigint.sa_flags = SA_RESTART;
		shell->sigint.sa_handler = child_signal_handler;
		sigemptyset(&shell->sigquit.sa_mask);
		shell->sigquit.sa_flags = SA_RESTART;
		shell->sigquit.sa_handler = child_signal_handler;
		sigaction(SIGINT, &shell->sigint, NULL);
	}
	else if (mode == PARENT_I)
	{
		sigemptyset(&shell->sigint.sa_mask);
		shell->sigint.sa_flags = SA_RESTART;
		shell->sigint.sa_handler = SIG_IGN;
		sigaction(SIGINT, &shell->sigint, NULL);
	}
	else if (mode == PARENT_O)
	{
		sigemptyset(&shell->sigint.sa_mask);
		shell->sigint.sa_flags = SA_RESTART;
		shell->sigint.sa_handler = handling_signal;
		sigaction(SIGINT, &shell->sigint, NULL);
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->terminal->minishell);
	}
}

static void	child_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		exit(130);
	}
	else if (signum == SIGQUIT)
	{
		printf("Quit: 3\n");
		exit(131);
	}
}
