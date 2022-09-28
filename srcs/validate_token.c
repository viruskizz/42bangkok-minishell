#include "minishell.h"

int	validate_token(t_list **tokens)
{
	t_list	*lst;

	lst = *tokens;
	while (lst)
	{
		if (!validate_opt(lst))
			return (0);
		lst = lst->next;
	}
	return (1);
}

/**
 * @brief 
 * 
 * @param lst 
 * @return int 0 is normal string, 1 is opt, -1 is invalid
 */
int	validate_opt(t_list *lst)
{
	int	is_t;

	is_t = is_opt((char *) lst->content);
	if (is_t < 0)
		return (0);
	if (is_t && !lst->next)
		return (0);
	if (is_t && is_opt((char *) lst->next->content))
		return (0);
	return (1);
}