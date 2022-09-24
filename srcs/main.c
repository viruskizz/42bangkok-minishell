#include "stdio.h"    
#include <readline/readline.h>
#include <readline/history.h>

int	main()
{
	char	*input;

	input = readline("input command\n");
	printf("input: %s\n", input);
	return (0);
}