
#include "minishell.h"

static char	**merge_input(int argc, char *argv[]);

int	main(int argc, char *argv[])
{
	char	*line;
	char	**words;

	if (line == NULL)
		return (EXIT_FAILURE);
	while(string_compare(line, "exit") == 0)
	{
		// line = readline("\x1B[5m\033[0;35mMI\033[0;36mNI\033[0;34mS\033[0;32mH\033[0;33mE\033[48;5;208mL\033[0;31mL\033[0m--> \x1B[0m");
		line = readline("input command\n");
		add_history(line);
		words = split_input(line);
		print_arr(words);
		// test_exe();
		// cmd_execution(shell);
	}
	return (EXIT_SUCCESS);
}

// int	main(int argc, char *argv[])
// {
// 	char	*line;
// 	char	**words;

// 	printf("n = %d\n", argc);
// 	if (argc == 1)
// 	{
// 		line = readline("input command\n");
// 		words = split_input(line);
// 	}
// 	else
// 	{
// 		words = merge_input(argc, argv);
// 	}
// 	print_arr(words);
// 	return (0);
// }

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