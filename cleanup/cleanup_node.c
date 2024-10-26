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

	// if (args->word)
		// {
		// 	free(args->word);
		// 	tmp = args->next;
		// 	free(args);
		// 	args = tmp;
		// }
		// else if (!args->next)
		// {
		// 	free(args);
		// 	args = NULL;
		// 	break ;
		// }

// void	cleanup_node_args(t_token *args)
// {
// 	int	i;
// 	t_token	*tmp;

// 	i = -1;
// 	if (!args)
// 		return ;
// 	while (args)
// 	{
//  		if (args->word)
// 		{
// 		 	free(args->word);
// 		 	tmp = args->next;
// 		 	free(args);
// 		 	args = tmp;
// 		}
// 		else if (!args->word)
// 		{
// 		 	free(args);
// 		 	args = NULL;
// 		 	break ;
// 		}
// 	}
// }

void	cleanup_node_args(t_token *args)
{
	if (args == NULL || args->word == NULL || args->kind == TK_EOF)
		return ;
	if (args->word != NULL)
		free(args->word);
	if (args->next != NULL)
		cleanup_node_args(args->next);
	free(args);
}


void	cleanup_node(t_node *node)
{
	t_node	*cur;
	t_node	*next;
	int		i;
	t_token	*args;
	t_token	*tmp;

	if (node == NULL)
		return ;
	cleanup_node_args(node->args);
	cleanup_redirect(node->redirect);
	cleanup_node(node->next);
	free(node);
}
