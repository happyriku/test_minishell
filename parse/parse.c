#include "../include/minishell.h"

t_node	*new_node(char *word, t_node_kind kind)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		fatal_error("malloc");
	node->kind = kind;
	node->next = NULL;
	node->redirect = NULL;
	return (node);
}

t_node *pipe_node(t_token **rest, char *word, t_node_kind kind)
{
	t_node	*pipeline;

	pipeline = malloc(sizeof(t_node));
	if (!pipe)
		return (NULL);
	if (pipe(pipeline->fd) == -1)
		fatal_error("pipe");
	return (pipeline);
}

t_node	*get_node(t_token **rest, t_token *token)
{
	t_node	*node;
	t_token	*args;
	t_node	*redirect;
	t_node	*pipeline;

	node = new_node(NULL, ND_SIMPLE_CMD);
	node->args = new_token(NULL, TK_EOF);
	if (!node->args)
		return (NULL);
	node->args = node->args->next;
	while (token)
	{
		if (token->kind == TK_EOF)
			break ;
		else if (token->kind == TK_WORD)
			append_token(&(node->args), new_token(token->word, TK_WORD));
		else if (strncmp(token->word, ">>", 2) == 0 && token->next->kind == TK_WORD)
			append_node(&(node->redirect), redirect_node(&token, token->next->word, ND_REDIRECT_APPEND));
		else if (strncmp(token->word, "<<", 2) == 0 && token->next->kind == TK_WORD)
		{
			if (token->next->next->word == NULL)
				append_node(&(node->redirect), heredoc_node(&token, token->next->word, ND_HEREDOC));
			else
				token = token->next;
		}
		else if (strcmp(token->word, ">") == 0 && token->next->kind == TK_WORD)
		 	append_node(&(node->redirect), redirect_node(&token, token->next->word, ND_REDIRECT_OUT));
		else if (strcmp(token->word, "<") == 0 && token->next->kind == TK_WORD)
		 	append_node(&(node->redirect), redirect_node(&token, token->next->word, ND_REDIRECT_IN));
		else if (strcmp(token->word, "|") == 0)
			break ;
		token = token->next;
	}
	append_token(&(node->args), new_token(NULL, TK_EOF));
	*rest = token;
	return (node);
}

t_node	*parse(t_token *token)
{
	t_node	*node;
	t_token	*args;

	node = get_node(&token, token);
	if (ft_lstsize(node->args) == 1)
	{
		g_info.syntax_error = true;
		printf("zsh: parse error\n");
	}
	if (token->word && strcmp(token->word, "|") == 0)
		node->next = parse(token->next);
	return (node);
}
