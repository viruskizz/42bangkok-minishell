/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 18:52:24 by araiva            #+#    #+#             */
/*   Updated: 2022/09/29 22:22:00 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		test_main();
static void		handling_signal(int signo);
static char		*handling_input(char *input);
static t_shell	parse_input(char *input);

// Testing main only
static int	test_main(char *src)
{
	char *line;
	char *test;
	t_shell shell;

	// test = ft_strdup("aaa bb dd << a >> b | $HOME \" xx $HOME\"");
	if (!src)
		test = ft_strdup("\"*\"");
	else
		test = ft_strdup(src);
	line = handling_input(test);
	shell = parse_input(line);
	free(line);
	if (shell.cmds)
		ft_lstclear(&shell.cmds, &free_cmd);
	return (0);
}

int	main(int argc, char *argv[])
{
	char				*line;
	char				*input;
	t_shell				shell;
	struct sigaction	sa;

	sa.sa_handler = handling_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);

	return (test_main(argv[1]));
	while (true)
	{
		input = readline(PROMPT_MSG);
		if (!input || ft_strcmp(input, "exit") == 0)
			break ;
		add_history(input);
		line = handling_input(input);
		if (!line)
			continue ;
		shell = parse_input(line);
		// test_exe();
		// cmd_execution(shell);
		free(line);
		ft_lstclear(&shell.cmds, &free_cmd);
	}
	return (EXIT_SUCCESS);
}

static t_shell	parse_input(char *input)
{
	t_shell	shell;
	t_list	*cmds;
	t_list	*tokens;

	printf("%sinput:%s: %s\n", CYAN, RESET, input);
	tokens = split_input(input);
	printf("%stokens: %s", CYAN, RESET);
	print_lst(tokens);
	if (!validate_token(&tokens))
	{
		printf("Error unexpected token\n");
		ft_lstclear(&tokens, &free_token);
		return (shell);
	}
	tokens = parse_token(tokens);
	printf("%sparsed: %s", CYAN, RESET);
	print_lst(tokens);
	cmds = group_cmd(tokens);
	printf("%stable command: %s\n", CYAN, RESET);
	print_cmd_table(cmds);
	shell.cmds = cmds;
	ft_lstclear(&tokens, &free_token);
	return shell;
}

static char	*handling_input(char *input)
{
	char	*line;

	line = ft_strtrim(input, " \t");
	free(input);
	if (ft_strlen(line) == 0)
		return (NULL);
	return (line);
}

static void	handling_signal(int signo)
{
	if (signo == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		ft_putstr_fd(PROMPT_MSG, STDOUT_FILENO);
	}
}
