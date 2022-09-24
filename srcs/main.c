
#include "minishell.h"

static char	**merge_input(int argc, char *argv[]);

int	main(int argc, char *argv[])
{
	char	*line;
	char	**words;

	printf("n = %d\n", argc);
	if (argc == 1)
	{
		line = readline("input command\n");
		words = split_input(line);
	}
	else
	{
		words = merge_input(argc, argv);
	}
	print_arr(words);
	return (0);
}

static char	**merge_input(int argc, char *argv[])
{
	int		i;
	char	**words;

	i = 0;
	words = ft_calloc(sizeof(char *), argc);
	while (++i < argc)
		words[i - 1] = ft_strdup(argv[i]);
	words[i] = NULL;
	return (words);
}