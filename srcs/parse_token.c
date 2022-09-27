#include "minishell.h"

static char	*parse_env(char *token);
static char	*var_val(char *str);
static int	word_len(char *token);
static int	var_len(char *s);

t_list	*parse_token(char **tokens)
{
	t_list	*word;
	t_list	*new;
	char	*str;

	word = NULL;
	while(*tokens)
	{
		if (*tokens[0] == '"')
			str = parse_env(*tokens);
		else
			str = ft_strdup(*tokens);
		new = ft_lstnew(str);
		if (!word)
			word = new;
		else
			ft_lstadd_back(&word, new);
		tokens++;
	}
	return (word);
}

static char	*parse_env(char *token)
{
	int		wlen;
	char	*str;
	t_list	*word;
	t_list	*new;

	word = NULL;
	printf("== parsing double quote ==\n");
	if (*token == '"')
		token++;
	while (*token && *token != '"')
	{
		if (*token == '$')
		{
			wlen = var_len(token);
			str = var_val(token);
		}
		else
		{
			wlen = word_len(token);
			str = ft_calloc(wlen + 1, sizeof(char));
			ft_strlcpy(str, token, wlen + 1);
			printf(">>%s\n", str);
		}
		new = ft_lstnew(str);
		if (!word)
			word = new;
		else
			ft_lstadd_back(&word, new);
		token += wlen;
	}
	print_lst(word);
	return (NULL);
}
/*
 * word length is including space char
 */
static int	word_len(char *s)
{
	int		i;
	char	*pattern;

	i = 0;
	pattern = "$\"\0";
	while (!ft_strchr(pattern, s[++i]));
	return (i);
}

/*
 * word length is not including space char
 */
static int	var_len(char *s)
{
	int		i;
	char	*pattern;

	i = 0;
	pattern = " $\"\t\0";
	if (*s != '$')
		return (i);
	while (!ft_strchr(pattern, s[++i]));
	return (i);
}

static char	*var_val(char *str)
{
	int		vlen;
	char	*var;
	char	*val;

	vlen = var_len(str);
	var = ft_calloc(vlen + 1, sizeof(char));
	ft_strlcpy(var, str, vlen + 1);
	printf("varname: %s\n", var);
	printf("varval: %s\n", getenv(var + 1));
	printf("varval: %s\n", getenv(var + 1));
	printf("varval: %s\n", getenv(var + 1));
	val = ft_strdup(getenv(var + 1));
	free(var);
	return (NULL);
}
