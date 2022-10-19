/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 18:52:24 by araiva            #+#    #+#             */
/*   Updated: 2022/10/16 15:53:48 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handling_input(char *input);
static int	parse_input(char *input, t_shell *shell);
static void	minishell_clear(t_shell *shell);

int	main(void)
{
	char				*line;
	char				*input;
	t_shell				shell;

	minishell_init(&shell);
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
		if (cmd_execution(&shell) < 0)
			perror("minishell");
		free(line);
		ft_lstclear(&shell.cmds, &free_cmd);
	}
	if (input == NULL)
		printf("exit\n");
	minishell_clear(&shell);
	environment_clear(&shell.env);
	exit(EXIT_SUCCESS);
}

/* fix add argument "shell" and change return value to int */
static int	parse_input(char *input, t_shell *shell)
{
	t_list	*cmds;
	t_list	*tokens;

	tokens = split_input(input);
	if (!validate_token(&tokens))
	{
		printf("Error unexpected token\n");
		ft_lstclear(&tokens, &free_token);
		return (-1);
	}
	tokens = parse_token(tokens, shell);
	cmds = group_cmd(tokens);
	shell->cmds = cmds;
	ft_lstclear(&tokens, &free_token);
	return (0);
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

static void	minishell_clear(t_shell *shell)
{
	rl_clear_history();
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->terminal->shell);
	free_db_ptr(NULL, NULL, shell->terminal);
}
