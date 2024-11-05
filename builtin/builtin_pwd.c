#include "../include/minishell.h"

int	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		fatal_error("getcwd");
	printf("%s\n", cwd);		
}