/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 18:52:24 by araiva            #+#    #+#             */
/*   Updated: 2022/10/24 17:28:21 by sharnvon         ###   ########.fr       */
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
		if (!input)
			break ;
		add_history(input);
		line = handling_input(input);
		if (!line)
			continue ;
		if (parse_input(line, &shell) == -1)
			continue ;
		if (cmd_execution(&shell) < 0)
			perror("minishell");
		ft_lstclear(&shell.cmds, &free_cmd);
	}
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
	free(input);
	if (!validate_token(&tokens))
	{
		ft_putstr_fd("minishell: Error unexpected token\n", 2);
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
	int		index;

	index = 0;
	while ((input[index] >= 9 && input[index] <= 13) || input[index] == ' ')
		index++;
	if (input[index] == '\0')
	{
		free(input);
		return (NULL);
	}
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
