/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 18:52:24 by araiva            #+#    #+#             */
/*   Updated: 2022/09/30 20:45:24 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		handling_signal(int signo);
static t_shell	handling_input(char *input);

int	main(int argc, char *argv[])
{
	// char				*line;
	// char				*input;
	// t_shell				shell;
	// struct sigaction	sa;

	// sa.sa_handler = handling_signal;
	// sigemptyset(&sa.sa_mask);
	// sa.sa_flags = SA_RESTART;
	// sigaction(SIGINT, &sa, NULL);
	// sigaction(SIGQUIT, &sa, NULL);
	// while (true)
	// {
	// 	input = readline(PROMPT_MSG);
	// 	if (!input || ft_strcmp(input, "exit") == 0)
	// 		break ;
	// 	add_history(input);
	// 	shell = handling_input(input);
	// 	if (ft_strlen(shell.line) == 0 || !shell.tokens)
	// 		continue ;
		test_execution();
		// cmd_execution(shell);
	// }
	return (EXIT_SUCCESS);
}

static t_shell	handling_input(char *input)
{
	t_shell	shell;
	t_list	*cmds;
	t_list	*tokens;
	char	*line;

	line = ft_strtrim(input, " \t");
	free(input);
	shell.line = line;
	if (ft_strlen(line) == 0)
		return shell;
	tokens = split_input(line);
	printf("%stokens: %s", CYAN, RESET);
	print_lst(tokens);
	if (!validate_token(&tokens))
	{
		printf("Error\n");
		ft_lstclear(&tokens, &free_token);
		return (shell);
	}

	parse_token(tokens);
	printf("%sparsed: %s", CYAN, RESET);
	print_lst(tokens);

	cmds = group_cmd(tokens);
	printf("%stable command: %s\n", CYAN, RESET);
	print_cmd_table(cmds);
	ft_lstclear(&tokens, &free_token);
	return shell;
}

static void	handling_signal(int signo)
{
	if (signo == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		ft_putstr_fd(PROMPT_MSG, STDOUT_FILENO);
	}
}
