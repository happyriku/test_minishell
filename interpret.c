#include "include/minishell.h"

extern char	**environ;
t_info g_info = {};

void	cleanup_token(t_token **token)
{
	t_token	*cur;
	t_token *next;

	cur = *token;
	while (cur)
	{
		next = cur->next;
		if (cur->word)
			free(cur->word);
		free(cur);
		cur = next;
	}
	*token = NULL;
}

int	interpret(char *input)
{
	int		status;
	char	*path;
	int		i;
	t_token	*token;
	t_token *tmp;
	t_info	info;
	t_node	*node;
	int		res;

	if (*input < 1)
		return (0);
	token = tokenize(input);
	if (!token)
		return (1);
	node = parse(token);
	if (g_info.syntax_error)
	{
		cleanup_token(&token);
		g_info.syntax_error = false;
		return (0);
	}
	if (!node)
		return (1);
	expand(node);
	if (g_info.syntax_error)
	{
		cleanup_token(&token);
		g_info.syntax_error = false;
		return (0);
	}
	res = exec(node);
	if (res == MALLOC_ERROR)
		return (cleanup_token(&token), 1);
	else if (res == -1)
		return (cleanup_token(&token), EXIT);
	else if (res == 0)
		return (cleanup_token(&token), 0);
	else if (res == 1)
	{
		cleanup_token(&token);
		exit(0);
	}
}
