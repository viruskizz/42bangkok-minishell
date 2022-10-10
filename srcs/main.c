/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 18:52:24 by araiva            #+#    #+#             */
/*   Updated: 2022/10/11 00:29:40 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct termios		attr;
struct termios		defu;

static int		test_main();
static void		handling_signal(int signo);
static char		*handling_input(char *input);
static int		parse_input(char *input, t_shell *shell);

// Testing main only
// static int	test_main(char *src)
// {
// 	char *line;
// 	char *test;
// 	t_shell shell;

// 	// test = ft_strdup("aaa bb dd << a >> b | $HOME \" xx $HOME\"");
// 	if (!src)
// 		test = ft_strdup("\"*\"");
// 	else
// 		test = ft_strdup(src);
// 	line = handling_input(test);
// 	shell = parse_input(line);
// 	free(line);
// 	if (shell.cmds)
// 		ft_lstclear(&shell.cmds, &free_cmd);
// 	return (0);
// }

int minishell_terminal_defualt(t_shell *shell)
{
	t_terminal	*terminal;

	terminal = shell->terminal;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal->shell);
	terminal->stty = (char **)ft_calloc(sizeof(char *), 3);
	if (terminal->stty == NULL)
		return (-1);
	terminal->stty[0] = ft_strdup("stty");
	if (terminal->stty[0] == NULL)
	{
		free_double_pointer(terminal->stty, NULL, terminal);
		return (-1);
	}
	terminal->stty[1] = ft_strdup("echoctl");
	if (terminal->stty[1] == NULL)
	{
		free_double_pointer(terminal->stty, NULL, terminal);
		return (-1);
	}
	execution_path_command(shell, terminal->stty);
	free_double_pointer(terminal->stty, NULL, terminal);
	return (0);
}

int	minishell_terminal_init(t_shell *shell)
{
	shell->terminal = (t_terminal *)ft_calloc(sizeof(t_terminal), 1);
	if (shell->terminal == NULL)
		return (-1);
	tcgetattr(STDIN_FILENO, &shell->terminal->minishell);
	tcgetattr(STDIN_FILENO, &shell->terminal->shell);
	shell->terminal->minishell.c_cc[VQUIT] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->terminal->minishell);
	shell->terminal->stty = (char **)ft_calloc(sizeof(char *), 3);
	if (shell->terminal->stty == NULL)
		return (-1);
	shell->terminal->stty[0] = ft_strdup("stty");
	if (shell->terminal->stty[0] == NULL)
	{
		free_double_pointer(shell->terminal->stty, NULL, shell->terminal);
		return (-1);
	}
	shell->terminal->stty[1] = ft_strdup("-echoctl");
	if (shell->terminal->stty[1] == NULL)
	{
		free_double_pointer(shell->terminal->stty, NULL, shell->terminal);
		return (-1);
	}
	execution_path_command(shell, shell->terminal->stty);
	free_double_pointer(shell->terminal->stty, NULL, NULL);
	return (0);
}

int	main(int argc, char *argv[])
{
	char				*line;
	char				*input;
	t_shell				shell;
	struct sigaction	sa;
	struct termios		attr;

	shell.sinput = dup(0);
	shell.exstat = 0;
	minishell_make_environment(&shell);
	if (minishell_terminal_init(&shell) < 0)
	{
		perror("minishell");
		environment_clear(&shell.env);
		return (EXIT_FAILURE);
	}

	sa.sa_handler = handling_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);

	// return (test_main(argv[1]));
	while (true)
	{
		input = readline(PROMPT_MSG);
		if (!input || ft_strcmp(input, "exit") == 0)
			break ;
		add_history(input);
		line = handling_input(input);
		if (!line)
			continue ;
		parse_input(line, &shell);
		printf("===================================== execution part ============================================\n\n");
		if (cmd_execution(&shell) < 0)
			perror("minishell: ");
		free(line);
		ft_lstclear(&shell.cmds, &free_cmd);
	}
	if (input == NULL)
		printf("exit\n");
	minishell_terminal_defualt(&shell);
	environment_clear(&shell.env);
	return (EXIT_SUCCESS);
}

/* fix add argument "shell" and change return value to int */
static int	parse_input(char *input, t_shell *shell)
{
	// t_shell	shell;
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
		return (-1);
	}
	tokens = parse_token(tokens);
	printf("%sparsed: %s", CYAN, RESET);
	print_lst(tokens);
	cmds = group_cmd(tokens);
	printf("%stable command: %s\n", CYAN, RESET);
	print_cmd_table(cmds);
	shell->cmds = cmds;
	ft_lstclear(&tokens, &free_token);
	return (0);
}

// static t_shell	parse_input(char *input)
// {
// 	t_shell	shell;
// 	t_list	*cmds;
// 	t_list	*tokens;

// 	printf("%sinput:%s: %s\n", CYAN, RESET, input);
// 	tokens = split_input(input);
// 	printf("%stokens: %s", CYAN, RESET);
// 	print_lst(tokens);
// 	if (!validate_token(&tokens))
// 	{
// 		printf("Error unexpected token\n");
// 		ft_lstclear(&tokens, &free_token);
// 		return (shell);
// 	}
// 	tokens = parse_token(tokens);
// 	printf("%sparsed: %s", CYAN, RESET);
// 	print_lst(tokens);
// 	cmds = group_cmd(tokens);
// 	printf("%stable command: %s\n", CYAN, RESET);
// 	print_cmd_table(cmds);
// 	shell.cmds = cmds;
// 	ft_lstclear(&tokens, &free_token);
// 	return shell;
// }

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
	// tcsetattr(STDIN_FILENO, TCSANOW, &attr);
	if (signo == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		// ft_putstr_fd(PROMPT_MSG, STDOUT_FILENO);
	}
	if (signo == SIGQUIT)
	{
		// ft_putchar_fd('\0', STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_redisplay();
	}
	// tcsetattr(STDIN_FILENO, TCSANOW, &defu);
}
