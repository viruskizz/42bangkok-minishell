/**
 * @brief Shell envioronment variable start with $ and following with alpha
 * @param s 
 * @return int 
 */
int	is_exp_var(char *s)
{
	int	len;

	len = ft_strlen(s);
	if (len < 2)
		return (0);
	if (s[0] == '$' && ft_isalpha(s[1]))
		return (1);
	else
		return (0);
}

/**
 * @brief Execute quote $?
 * @return int length of var name (2)
 */
int	is_ext_var(char *s)
{
	int	len;

	len = ft_strlen(str);
	if (len < 2)
		return (0);
	if (s[0] == '$' && s[1] == '?')
		return (2);
	else
		return (0);
}

/**
 * @brief process id, quote is $$
 * @return int length of var name (2)
 */
int	is_pid_var(char *s)
{
	int	len;

	len = ft_strlen(str);
	if (len < 2)
		return (0);
	if (s[0] == '$' && s[1] == '$')
		return (2);
	else
		return (0);
}

/**
 * @brief the name is shell script, quote is $0
 * @return int length of var name (2)
 */
int	is_zero_var(char *s)
{
	int	len;

	len = ft_strlen(str);
	if (len < 2)
		return (0);
	if (s[0] == '$' && s[1] == '$')
		return (2);
	else
		return (0);
}

/**
 * @brief Execute quote $(...)
 * @return int length of command
 */
int	is_exq_var(char *s)
{
	int	len;

	len = ft_strlen(str);
	if (len < 3)
		return (0);
	if (s[0] != '$' && s[1] != '(')
		return (0);
	len = 0;
	while (s[len])
		if (s[len] == ')')
			return (len);
	return (0);
}