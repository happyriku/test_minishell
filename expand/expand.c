#include "../include/minishell.h"

void	append_char(char **new_word, char word)
{
	char	*memory;
	size_t	len;

	len = strlen(*new_word) + 2;
	printf("len : %zu\n", len);
	memory = (char *)malloc(sizeof(char) * (len));
	if (!memory)
		return ;
	ft_strncpy(memory, *new_word, len);
	memory[len - 2] = word;
	memory[len - 1] = '\0';
	free(*new_word);
	*new_word = memory;
}

bool	is_closed_quote(char *word)
{
	int	single_quote_count;
	int	double_quote_count;

	single_quote_count = 0;
	double_quote_count = 0;
	while (*word)
	{
		if (*word == SINGLEQUOTE)
			single_quote_count++;
		if (*word == DOUBLEQUOTE)
			double_quote_count++;
		word++;
	}
	if (single_quote_count % 2 != 0 || double_quote_count % 2 != 0)
		return (false);
	return (true);
}

void	syntax_error(void)
{
	g_info.syntax_error = true;
	printf("syntax error\n");
}

// void	quote_removal(t_token *args)
// {
// 	char	**word;
// 	char	*new_word;

// 	if (args->next == NULL)
// 		return ;
// 	if (!is_closed_quote(args->word))
// 		syntax_error();
// 	word = &(args->word);
// 	new_word = calloc(1, sizeof(char));
// 	if (!new_word)
// 		fatal_error("malloc");
// 	while (**word && !is_metacharacter(**word))
// 	{
// 		if (**word != SINGLEQUOTE && **word != DOUBLEQUOTE)
// 			append_char(&new_word, **word);
// 		(*word)++;
// 	}
// 	free(args->word);
// 	args->word = new_word;
// 	quote_removal(args->next);
// }


bool	is_alpha_under(char c)
{
	return (ft_isalpha(c) || c == '_');
}

bool	is_variable(char *word)
{
	return (*word == '$' && is_alpha_under(*(word + 1)));
}

bool	handle_double_quote(char **new_word, char **rest, char *p)
{
	char	*path;

	p++;
	while (*p && *p != DOUBLEQUOTE)
	{
		if (is_variable(p))
			p++;
		append_char(new_word, *(p++));
	}
	if (*p == '\0')
		syntax_error();
	printf("new_word : %s\n", *new_word);
	path = getenv(*new_word);
	printf("path : %s\n", path);
	*rest = p;
}

void	handle_single_quote(char **new_word, char **rest, char *p)
{
	p++;
	while (*p && *p != SINGLEQUOTE)
		append_char(new_word, *(p++));
	if (*p == '\0')
	{
		printf("new_word : %s\n", *new_word);
		syntax_error();
	}
	if (*p == SINGLEQUOTE)
		p++;
	*rest = p;
}

void	expand_variable(t_token *args)
{
	char	*new_word;
	char	*p;

	if (!args || args->kind == TK_EOF || args->word == NULL)
		return ;
	p = args->word;
	new_word = calloc(1, sizeof(char));
	if (!new_word)
		fatal_error("malloc");
	while (*p && !is_metacharacter(*p))
	{
		if (*p == SINGLEQUOTE)
			handle_single_quote(&new_word, &p, p);
		else if (*p == DOUBLEQUOTE)
			handle_double_quote(&new_word, &p, p);
		else
			append_char(&new_word, *(p++));
	}
	free(args->word);
	args->word = new_word;
	expand_variable(args->next);
}

void	expand(t_node *node)
{
	t_token	*args;

	args = node->args;
	expand_variable(args);
}
