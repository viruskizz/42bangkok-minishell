/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiva <tsomsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/16 23:07:21 by araiva            #+#    #+#             */
/*   Updated: 2022/10/16 23:07:22 by araiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	minishell_terminal(t_shell *shell);

void	minishell_init(t_shell *shell)
{
	shell->sinput = dup(0);
	shell->exstat = 0;
	shell->sigint.sa_handler = handling_signal;
	sigemptyset(&shell->sigint.sa_mask);
	shell->sigint.sa_flags = SA_RESTART;
	sigaction(SIGINT, &shell->sigint, NULL);
	shell->sigquit.sa_handler = SIG_IGN;
	sigemptyset(&shell->sigquit.sa_mask);
	shell->sigquit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &shell->sigquit, NULL);
	minishell_make_environment(shell);
	if (minishell_terminal(shell) < 0)
	{
		perror("minishell");
		environment_clear(&shell->env);
		exit(EXIT_FAILURE);
	}
}

void	minishell_clear(t_shell *shell)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->terminal->shell);
	free_db_ptr(NULL, NULL, shell->terminal);
}

static int	minishell_terminal(t_shell *shell)
{
	shell->terminal = (t_terminal *)ft_calloc(sizeof(t_terminal), 1);
	if (shell->terminal == NULL)
		return (-1);
	tcgetattr(STDIN_FILENO, &shell->terminal->minishell);
	tcgetattr(STDIN_FILENO, &shell->terminal->shell);
	shell->terminal->minishell.c_lflag &= ~ECHOCTL;
	shell->terminal->shell.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->terminal->minishell);
	return (0);
}
