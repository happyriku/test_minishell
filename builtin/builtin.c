#include "../include/minishell.h"

int	get_size(t_token *args)
{
	t_token	*tmp;
	int		count;

	tmp = args;
	count = 0;
	while (tmp && tmp->kind != TK_EOF)
	{
		tmp = tmp->next;
		count++;
	}
	return (count);
}

char	**create_args(t_token *token)
{
	char	**args;
	size_t	len;
	int		size;
	int		i;

	size = get_size(token);
	args = (char **)malloc(sizeof(char *) * (size + 1));
	if (!args)
		fatal_error("mallo");
	i = -1;
	while (token && token->kind != TK_EOF)
	{
		args[++i] = (char *)malloc(sizeof(char) * strlen(token->word) + 1);
		if (!args)
			fatal_error("malloc");
		ft_strncpy(args[i], token->word, strlen(token->word) + 1);
		token = token->next;
	}
	args[++i] = NULL;
	return (args);
}

int	exec_builtin(t_node *node)
{
	char	**args;
	int		status;
	if (node->redirect)
		do_redirect(node->redirect);
	args = create_args(node->args);
	if (strcmp(args[0], "echo") == 0)
		status = builtin_echo(args);
	else if (strcmp(args[0], "exit") == 0)
		status = builtin_exit(args);
	else if (strcmp(args[0], "pwd") == 0)
		status = builtin_pwd();
	if (node->redirect)
		reset_redirect(node->redirect);
	return (status);
}
