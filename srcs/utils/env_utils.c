/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_environment.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 18:52:47 by sharnvon          #+#    #+#             */
/*   Updated: 2022/10/14 23:27:04 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* function make new node of link-list of t_env */
t_env	*environment_new(char *env)
{
	t_env	*result;

	result = (t_env *)ft_calloc(sizeof(t_env), 1);
	if (result == NULL)
	{
		return (0);
	}
	result->name = environment_get_name(env);
	result->value = environment_get_value(env);
	result->next = NULL;
	return (result);
}

/* function add node of back of link-list of t_env */
void	environment_add_back(t_env **env, t_env *new)
{
	t_env	*temp;

	temp = *env;
	if (*env == NULL)
		*env = new;
	else
	{
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = new;
	}
}

void	environment_clear(t_env **env)
{
	t_env	*temp;

	temp = *env;
	if (env != NULL)
	{
		while (temp != NULL)
		{
			*env = (*env)->next;
			free(temp->name);
			free(temp->value);
			free(temp);
			temp = *env;
		}
	}
}

/* function get the value of t_env */
/* funntion working same with getenv() */
char	*environment_getenv(char *variable_name, t_shell *shell)
{
	t_env	*envs;

	envs = shell->env;
	while (envs != NULL)
	{
		if (string_compare(variable_name, envs->name, NO_LEN) == 1)
		{
			return (envs->value);
		}
		envs = envs->next;
	}
	return (0);
}

/* function get the environment */
int	minishell_make_environment(t_shell *shell)
{
	t_env	*new;
	int		index;

	index = 0;
	shell->env = NULL;
	while (environ[index] != NULL)
	{
		new = environment_new(environ[index]);
		if (new == NULL)
		{
			environment_clear(&shell->env);
			perror("minishell");
			exit (1);
		}
		environment_add_back(&shell->env, new);
		index++;
	}
	return (0);
}
