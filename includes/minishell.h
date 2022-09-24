/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 22:58:34 by sharnvon          #+#    #+#             */
/*   Updated: 2022/09/24 05:26:20 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h> 
# include <readline/readline.h>
# include <readline/history.h>

# define OPT_NULL	0
# define OPT_PIPE	1
# define OPT_AND	2
# define OPT_OR		3

/* redirect flag in t_cmd */
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

/**
 * @brief struct for single command with conjuction
 * exmaple input: ls -l && wc -l
 * for index 0
 * cmd = "ls -l"
 * conj = CONJ_AND
 */
// typedef struct s_cmd
// {
// 	char	*cmd;
// 	char	*file;
// 	int		opt;
// 	int		redir;
// }	t_cmd;

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
	int		cmd_amount;
}	t_shell;


extern char	**environ;

int		string_compare(char *str1, char *str2);
int		character_search(char *str, char c, int mode);
int		ft_lencount(char *str, char **strs, int mode);
// void	*ft_calloc(int count, int size);
char	**ft_split_mode(char *str, char c, int mode);
char	*ft_midjoin(char *str1, char *str2, char c);

char	**split_input(char *line);

// utility
void	print_arr(char **str);
int		is_opt(char *str);
int		is_spt(char c);
int		is_end_quote(char c);

#endif
