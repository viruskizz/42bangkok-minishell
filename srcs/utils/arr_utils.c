#include "minishell.h"

char	**lst_to_arr_(t_list *lst)
{
	int		i;
	int		n;
	t_list	*tmp;
	char	**arr;

	n = ft_lstsize(lst);
	arr = ft_calloc(n + 1, sizeof(char *));
	i = 0;
	tmp = lst;
	while (i < n)
	{
		arr[i++] = ft_strdup(lst->content);
		lst = lst->next;
	}
	arr[i] = NULL;
	return (arr);
}