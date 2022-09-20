/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 22:58:34 by sharnvon          #+#    #+#             */
/*   Updated: 2022/09/20 20:36:21 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <stdlib.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>

// mode in count_word //
# define WORD 1
# define LETTER 2

// mode in ft_split //
# define FREE 111
# define BOUND 222

// mode in ft_strchar //
# define FRONT 1111
# define BACK 2222

extern char	**environ;

int		string_compare(char *str1, char *str2);
int		character_search(char *str, char c, int mode);
int		ft_lencount(char *str);
void	*ft_calloc(int count, int size);
char	**ft_split(char *str, char c, int mode);
char	*ft_strjoin(char *str1, char *str2, char c);

#endif
