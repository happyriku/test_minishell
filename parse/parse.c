#include "../include/minishell.h"

t_node	*new_node(char *word, t_node_kind kind)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->kind = kind;
	node->next = NULL;
	node->redirect = NULL;
	return (node);
}

int	init_node(t_node **node)
{
	*node = new_node(NULL, ND_SIMPLE_CMD);
	if (!(*node))
		return (0);
	(*node)->args = new_token(NULL, TK_EOF);
	if (!(*node)->args)
		return (0);
	(*node)->args = (*node)->args->next;
	return (1);
}

t_node	*get_node(t_token *token)
{
	t_node	*node;
	t_token	*args;
	t_node	*redirect;

	if (!init_node(&node))
		return (NULL);
	while (token)
	{
		if (token->kind == TK_EOF)
			break ;
		else if (token->kind == TK_WORD)
			append_token(&(node->args), new_token(token->word, TK_WORD));
		else if (strncmp(token->word, ">>", 2) == 0 && token->next->kind == TK_WORD)
			append_node(&(node->redirect), new_redirect_node(&token, token->next->word, ND_REDIRECT_APPEND));
		else if (strncmp(token->word, "<<", 2) == 0 && token->next->kind == TK_WORD)
		{
			if (token->next->next->word == NULL)
				append_node(&(node->redirect), new_heredoc_node(&token, token->next->word, ND_HEREDOC));
			else
				token = token->next;
		}
		else if (strcmp(token->word, ">") == 0 && token->next->kind == TK_WORD)
		 	append_node(&(node->redirect), new_redirect_node(&token, token->next->word, ND_REDIRECT_OUT));
		else if (strcmp(token->word, "<") == 0 && token->next->kind == TK_WORD)
		 	append_node(&(node->redirect), new_redirect_node(&token, token->next->word, ND_REDIRECT_IN));
		token = token->next;
	}
	append_token(&(node->args), new_token(NULL, TK_EOF));
	return (node);
}

t_node	*parse(t_token *token)
{
	t_node	*node;
	t_token	*args;
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		if (strcmp(tmp->word, "|") == 0)
			printf("token->kind : %d\n", tmp->kind);
		tmp = tmp->next;
	}
	node = get_node(token);
	if (ft_lstsize(node->args) == 1)
	{
		g_info.syntax_error = true;
		printf("zsh: parse error\n");
	}
	return (node);
}
