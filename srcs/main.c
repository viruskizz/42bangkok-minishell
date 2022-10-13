/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 18:52:24 by araiva            #+#    #+#             */
/*   Updated: 2022/10/13 22:29:30 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		test_main();
// static void		handling_signal(int signo);
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

int	minishell_terminal(t_shell *shell, int mode)
{
	if (mode == INIT)
	{
		shell->terminal = (t_terminal *)ft_calloc(sizeof(t_terminal), 1);
		if (shell->terminal == NULL)
			return (-1);
		tcgetattr(STDIN_FILENO, &shell->terminal->minishell);
		tcgetattr(STDIN_FILENO, &shell->terminal->shell);
		shell->terminal->minishell.c_lflag &= ~ ECHOCTL;
		shell->terminal->shell.c_lflag |= ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->terminal->minishell);
	}
	if (mode == DEFUALT)
	{
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->terminal->shell);
		free_double_pointer(NULL , NULL, shell->terminal);
	}
	return (0);
}

int	minishell_init(t_shell *shell)
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
	if (minishell_terminal(shell, INIT) < 0)
	{
		perror("minishell");
		environment_clear(&shell->env);
		exit (EXIT_FAILURE);
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	char				*line;
	char				*input;
	t_shell				shell;
	
	minishell_init(&shell);
	// return (test_main(argv[1]));
	if (argc > 1)
	{
		int		index = 1;
		char	*tmp = NULL;

		while (argv[index] != NULL)
		{
			input = ft_midjoin(tmp, argv[index++], ' ');
			if (tmp != NULL)
				free(tmp);
			tmp = ft_strdup(input);
		}
		free(tmp);
		printf("input = %s\n", input);
		if (!input || ft_strcmp(input, "exit") == 0)
				return (0);
		add_history(input);
		line = handling_input(input);
		if (!line)
			return (0);
		parse_input(line, &shell);
		printf("===================================== execution part ============================================\n\n");
		if (cmd_execution(&shell) < 0)
			perror("minishell");
		free(line);
		ft_lstclear(&shell.cmds, &free_cmd);
	}
	else
	{
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
				perror("minishell");
			free(line);
			ft_lstclear(&shell.cmds, &free_cmd);
		}
		// ! DONT FOGET DELETE IT OUT
		if (input == NULL)
		{
			printf("\033[A");
			for (int i = 0; i < 15; i ++)
				printf("%s", DELNL);
			printf("exit\n");
		}
	}
	minishell_terminal(&shell, DEFUALT);
	environment_clear(&shell.env);
	exit(EXIT_SUCCESS);
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
	tokens = parse_token(tokens, shell);
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

void	handling_signal(int signo)
{
	if (signo == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
