#include "../include/minishell.h"

int	exec(t_node *node)
{
    int status;

    if (!node->next && is_builtin(node->args))
        status = exec_builtin(node);
    else
        status = exec_command(node, STDIN_FILENO);
    return (status);
}
