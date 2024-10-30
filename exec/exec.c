#include "../include/minishell.h"

extern char **environ;

int	exec(t_node *node)
{
    int status;
// 	if (strncmp(node->args->word, "exit", 4) == 0)
// 		return (printf("exit\n"), -1);
    if (!node->next && is_builtin(node->args))
        status = exec_builtin(node);
    else
        status = exec_command(node, STDIN_FILENO);
    return (status);
}
