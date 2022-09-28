#include "minishell.h"

t_list	*wild_paths(t_list *tokens)
{
	t_list	*paths;
	t_list	*new;
	char	*dirname;
	DIR *dir;
	struct dirent *entry;

	paths = NULL;
	if ((dir = opendir("/home/araiva/42bangkok/minishell/minishell")) == NULL)
		perror("opendir() error");
	else {
		while ((entry = readdir(dir)) != NULL)
		{
			dirname = ft_strdup(entry->d_name);
			new = ft_lstnew(dirname);
			if (!paths)
				paths = new;
			else
				ft_lstadd_back(&paths, new);
		}
		closedir(dir);
	}
	return (paths);
}
