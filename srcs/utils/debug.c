#include "minishell.h"

void	print_arr(char **str)
{
	int i;

	i = 0;
	ft_putstr_fd("{", STDOUT_FILENO);
	while (*str)
	{
		ft_putstr_fd(*str, STDOUT_FILENO);
		if (*(str + 1))
			ft_putstr_fd(", ", STDOUT_FILENO);
		str++;
	}
	ft_putstr_fd("}\n", STDOUT_FILENO);
}