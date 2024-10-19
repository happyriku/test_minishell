#include "../include/minishell.h"

void	cleanup_token(t_token *token)
{
	t_token	*cur;
	t_token *next;

	//cur = *token;
	while (token)
	{
		next = token->next;
		if (token->word)
			free(token->word);
		free(token);
		token = next;
	}
	token = NULL;
}
