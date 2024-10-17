#include "../include/minishell.h"

extern char **environ;

void	append_argv(t_token **args)
{
    int argc;
    t_token *cur;

    cur = *args;
    argc = 0;
    while (cur && cur->kind != TK_EOF)
    {
        argc++;
        cur = cur->next;
    }
    (*args)->arr = (char **)malloc(sizeof(char *) * (argc + 1));
    if (!(*args)->arr)
        fatal_error("malloc");
    argc = 0;
    cur = *args;
    while (cur && cur->kind != TK_EOF)
    {
        (*args)->arr[argc] = strdup(cur->word);
        if (!((*args)->arr[argc]))
            fatal_error("malloc");
        cur = cur->next;
        argc++;
    }
    (*args)->arr[argc] = NULL; 
    (*args)->next->arr = NULL;
}

int	exec(t_node *node)
{
	char	**argv;
	pid_t	pid;
	char	*path;
	int		i;
	int		status;
    t_node *cur;

    cur = node;
    while (cur)
    {
        append_argv(&cur->args);
        cur = cur->next;
    }
	if (strncmp(node->args->arr[0], "exit", 4) == 0)
		return (printf("exit\n"), -1);
	return (exec_command(node, STDIN_FILENO));
}
