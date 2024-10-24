#include "../include/minishell.h"

void	cleanup_redirect(t_node *redirect)
{
	t_node	*cur;

	if (!redirect)
		return ;
	while (redirect)
	{
		if (redirect->filename)
			free(redirect->filename);
		if (redirect->delimiter)
			free(redirect->delimiter);
		if (!redirect->next)
			return ;
		cur = redirect->next;
		free(redirect);
		redirect = cur;
	}
}

void	cleanup_node_args(t_token *args)
{
	int	i;
	t_token	*tmp;

	i = -1;
	if (!args)
		return ;
	while (args)
	{
		while (args->arr && args->arr[++i])
			free(args->arr[i]);
		if (args->arr)
			free(args->arr);
		if (args->word)
			free(args->word);
		if (args->next)
		{
			tmp = args->next;
			free(args);
			args = tmp;
		}
		if (!args->next)
		{
			args = NULL;
			break ;
		}
	}
}

void	cleanup_node(t_node *node)
{
	t_node	*cur;
	t_node	*next;
	int		i;
	t_token	*args;
	t_token	*tmp;

	if (!node)
		return ;
	cleanup_node_args(node->args);
	cleanup_node(node->redirect);
	cleanup_node(node->next);
	free(node);
}
