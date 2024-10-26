#include "../include/minishell.h"

void	append_char(char **new_word, char word)
{
	char	*memory;
	size_t	len;

	if (*new_word == NULL)
		len = 2;
	else
		len = strlen(*new_word) + 2;
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

char	*get_path(char *word)
{
	char	*path;

	path = strdup(getenv(word));
	if (!path)
		return (NULL);
	free(word);
	return (path);
}

bool	handle_double_quote(char **new_word, char **rest, char *p)
{
	char	*path;
	bool	var_flag;

	p++;
	while (*p && *p != DOUBLEQUOTE)
	{
		if (is_variable(p))
		{
			var_flag = true;
			p++;
		}
		append_char(new_word, *(p++));
	}
	if (*p == '\0')
		syntax_error();
	if (*p == DOUBLEQUOTE)
		p++;
	if (var_flag)
		*new_word = get_path(*new_word);
	*rest = p;
}

void	handle_single_quote(char **new_word, char **rest, char *p)
{
	p++;
	while (*p && *p != SINGLEQUOTE)
		append_char(new_word, *(p++));
	if (*p == '\0')
		syntax_error();
	if (*p == SINGLEQUOTE)
		p++;
	*rest = p;
}

void	handle_variable(char **new_word, char **rest, char *p)
{
	char	*str;

	str = calloc(1, sizeof(char));
	if (!str)
		fatal_error("malloc");
	while (*p && *p != SINGLEQUOTE && *p != DOUBLEQUOTE)
	{
		if (*p == '$')
			p++;
		append_char(&str, *(p++));
	}
	if (*p == SINGLEQUOTE || *p == DOUBLEQUOTE)
	{
		p++;
		syntax_error();
	}
	else
		*new_word = get_path(str);
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
		else if (is_variable(p))
			handle_variable(&new_word, &p, p);
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
