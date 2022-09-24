#include "minishell.h"

int	is_end_quote(char c)
{
	if (c == '"' || c == '\'')
		return (1);
	else
		return (0);
}

int	is_opt(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (len > 2 || len == 0)
		return (OPT_NULL);
	if (strcmp(str, "&&"))
		return (OPT_AND);
	if (strcmp(str, "||"))
		return (OPT_OR);
	if (strcmp(str, "|"))
		return (OPT_PIPE);
	return (OPT_NULL);
}

int	is_spt(char c)
{
	if (c == ' ' || c == '\n' || c == '\t')
		return (1);
	else
		return (0);
}