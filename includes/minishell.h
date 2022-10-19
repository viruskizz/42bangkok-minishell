/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 22:58:34 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/14 22:26:18 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "color.h"
# include <stdbool.h>  
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h> 
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <sys/stat.h>
# include <termios.h>
# include <sys/types.h>
# include <sys/wait.h>

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
# define FREE	1
# define BOUND	2

/* mode in ft_strchar */
# define FRONT	1
# define BACK	2

/* mode in ft_lencount */
# define STR	1
# define STRS	2

/* mode in environment_check_value*/
# define START	1
# define QUOTE	2
# define INDEX	3

/* mode in string_compare */
# define NO_LEN	-42

/* mode in execution_signal */
# define CHILD		0
# define PARENT_I	1
# define PARENT_O	2

# define INIT		1
# define DEFUALT	2

# define DELNL "\033[C"
# define FIELDS	" \t\n\0"
# define QUOTES	"'\""

# define IN_FILE ".temporary_for_collecting_infile_u_cannot_see_this_3saatoooo"

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_cmd
{
	char	**tokens;
	char	**fg;
	char	**fgg;
	char	**fls;
	char	**flsls;
	int		opt;
}	t_cmd;

typedef struct s_execute
{
	int			index;
	int			xedni;
	int			files;
	int			execute;
	int			fd[2];
	pid_t		pid;
	t_cmd		*cmds;
	struct stat	info;
}	t_execute;

typedef struct s_lcmd
{
	t_list	*tokens;
	t_list	*fgt;
	t_list	*fgtgt;
	t_list	*fls;
	t_list	*flsls;
	int		opt;
}	t_lcmd;

typedef struct s_token
{
	char	**tokens;
	char	**fg;
	char	**fgg;
	char	**fls;
	char	**flsls;
	int		opt;
}	t_token;

typedef struct s_terminal
{
	struct termios	minishell;
	struct termios	shell;

}	t_terminal;

/**
 * @brief struct for entire shell programe
 */
typedef struct s_shell
{
	char				*line;
	t_token				*tokens;
	t_env				*env;
	t_list				*cmds;
	t_terminal			*terminal;
	int					exstat;
	int					sinput;
	struct sigaction	sigint;
	struct sigaction	sigquit;
}	t_shell;

extern char	**environ;

char	*string_tranfer(char *str1, char *str2, int len);
int		string_compare(char *str1, char *str2, int len);
int		character_search(char *str, char c, int mode);
int		ft_lencount(char *str, char **strs, int mode);

char	**ft_split_mode(char *str, char c, int mode);
char	*ft_midjoin(char *str1, char *str2, char c);
char	**doublepointer_join(char **strs, char *str);
int		redirect_dup_start(t_shell *shell, t_execute *exe);
void	execution_signal(t_shell *shell, int mode);
// excution part //
int		cmd_execution(t_shell *shell);
int		execution_command(t_shell *shell, t_execute *exe, t_cmd *cmds);
int		execution_token(t_shell *shell, char *path, char **command);
int		minishell_redirect(t_shell *shell, int *fd, int index);
int		execution_path_command(t_shell *shell, char **command, int index);

//	execution_export_env //
int		execution_export_env(t_shell *shell, char **command, int index);
char	*environment_get_name(char *command);
char	*environment_get_value(char *command);
int		environment_check_value(char *command, int quote, int qquote, int mode);
int		environment_check_name(char *variable_name, char *cmd, t_shell *shell);
char	*environment_getenv(char *variable_name, t_shell *shell);

int		environment_export_env(
			t_shell *shell, char *name, char *value, char *cmd);
int		execution_unset_env(t_env **env, char **variable_name, int index);
void	environment_delete(t_env *env);
void	environment_delete(t_env *env);

int		minishell_make_environment(t_shell *shell);
t_env	*environment_new(char *env);
void	environment_add_back(t_env **env, t_env *new);
void	environment_clear(t_env **env);
int		execution_print_env(t_shell *shell);

// redirect
int		redirect_infile(t_shell *shell, t_cmd *cmds);
char	*heredoc_convert_env(t_shell *shell, char *buff, int index, int xedni);

int		free_db_ptr(char **str1, char **str2, void *str3);
void	signal_defualt(void);
int		execution_change_directory(t_shell *shell, char **command);

// main function
void	minishell_init(t_shell *shell);
t_list	*split_input(char *line);
t_list	*parse_token(t_list *tokens, t_shell *shell);
t_list	*group_cmd(t_list *token);
int		validate_token(t_list **tokens);
void	free_cmd(void *content);
void	free_token(void *content);

// utility
int		exp_env(char *token, char **str, t_shell *shell);
int		exp_str(char *token, char **str);
int		exp_env_hom(char *token, char **str, t_shell *shell);
t_list	*wild_paths(t_list *tokens);

int		is_opt(char *str);
int		parse_opt(char *opt);
char	*str_opt(int opt);
int		is_redirect(char *str);
int		is_sq_str(char	*s);
int		is_dq_str(char	*s);
int		is_exp_var(char *s);
int		is_wild_var(char *s);

char	**lst_to_arr(t_list *lst);
int		arr_str_len(char **arr);
void	free_arr(char **arr);

// debug
void	print_arr(char **str);
void	print_lst(t_list *lst);
void	print_cmd_table(t_list *cmds);
void	handling_signal(int signo);

#endif
