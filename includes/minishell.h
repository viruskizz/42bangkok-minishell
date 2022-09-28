/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 22:58:34 by sharnvon          #+#    #+#             */
/*   Updated: 2022/09/26 02:14:29 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdbool.h>  
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h> 
# include <readline/readline.h>
# include <readline/history.h>

# define PROMPT_MSG	"\033[1;33minput command: \033[0m"
# define OPT_NULL	0
# define OPT_PIPE	1
# define OPT_AND	2
# define OPT_OR		3

/* redirect flag in t_cmd */
# define REDI_NULL	0 // no redirect;
# define FROM		1 // <  
# define FFROM		2 // <<
# define TO			3 // >
# define TTO		4 // >>

/* mode in count_word */
# define WORD	1
# define LETTER	2

/* mode in ft_split */
# define FREE	111
# define BOUND	222

/* mode in ft_strchar */
# define FRONT	1111
# define BACK	2222

/* mode in ft_lencount */
# define STR	1
# define STRS	2

# define FIELDS	" \t\n"
# define QUOTES	"'\""

typedef struct s_word
{
	char			*str;
	struct s_word	*next;
}	t_word;

# define HERE_DOC ".temporary_file_for_heredoc_u_can_not_see_this_please_saatoo"

/**
 * @brief struct for single command with conjuction
 * exmaple input: ls -l && wc -l
 * for index 0
 * cmd = "ls -l"
 * conj = CONJ_AND
 */
typedef struct s_cmd
{
	char	*token;
	char	*file;
	int		n;
	int		opt;
	int		redir;
}	t_cmd;

typedef struct s_token
{
	char	*token;
	char	*file;
	int		opt;
	int		redir;
}	t_token;

/**
 * @brief struct for entire shell programe
 */
typedef struct	s_shell
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;
	int		cmd_amount;
}	t_shell;


extern char	**environ;

int		string_compare(char *str1, char *str2);
int		character_search(char *str, char c, int mode);
int		ft_lencount(char *str, char **strs, int mode);
// void	*ft_calloc(int count, int size);
char	**ft_split_mode(char *str, char c, int mode);
char	*ft_midjoin(char *str1, char *str2, char c);
int		test_execution(void);

// excution part //
int		minishell_redirect(t_shell *shell, int *fd, int index);

t_list	*split_input(char *line);
t_cmd	*group_cmd(t_list *token);
t_list	*parse_token(char **tokens);
void	free_token(void *content);

// utility
void	print_arr(char **str);
void	print_lst(t_list *lst);
int		is_opt(char *str);
int	validate_opt(t_list *lst);

#endif
