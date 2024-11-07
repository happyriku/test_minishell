#include "../include/minishell.h"

int	builtin_unset(char	**args)
{
	if (unsetenv(args[1]) != 0)
		return (1);
	return (0);
}