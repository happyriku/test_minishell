#include "../include/minishell.h"

void	fatal_error(char *msg)
{
	printf("%s\n", msg);
	exit(1);
}

char	*filter_operator(char *input, char **rest)
{
	char	*op[] = {"||", "&&", "(", ")", ";;", ";&", "&", ";", "|&", "\n"};
	int		i;
	int		length;
	char	*str;

	length = sizeof(op) / sizeof(op[0]);
	i = -1;
	while (++i < length)
	{
		if (strncmp(input, op[i], strlen(op[i])) == 0)
		{
			(*rest) += strlen(op[i]);
			str = ft_strndup(op[i], strlen(op[i]));
			if (!str)
				fatal_error("malloc error");
			return (str);
		}
	}
	return (NULL);
}

char	*filter_metacharacter(char c, char **rest)
{
	char	*res;
	char	*str;

	res = strchr("|&;()<>\t\n", c);
	(*rest)++;
	str = ft_strndup(res, 1);
	if (!str)
		fatal_error("malloc error");
	return (str);
}

char	*filter_word(char *input, char **rest)
{
	char	*str;

	while (**rest && is_word(*rest))
	{
		if (**rest == SINGLEQUOTE || **rest == DOUBLEQUOTE)
		{
			(*rest)++;
			while (**rest && **rest != SINGLEQUOTE && **rest != DOUBLEQUOTE)
			{
				if (**rest == '\0')
					printf("unclosed single quote\n");
				(*rest)++;
			}
		}
		else
			(*rest)++;
	}
	str = ft_strndup(input, *rest - input);
	if (!str)
		fatal_error("malloc error");
	return (str);
}

t_token	*tokenize(char *input)
{
	t_token	head;
	t_token	*token;

	head.next = NULL;
	token = &head;
	while (*input)
	{
		while (is_blank(*input))
			input++;
		if (is_ctrlop(input))
			token->next = new_token(filter_operator(input, &input), TK_OPERATOR);
		else if (is_metacharacter(*input))
			token->next = new_token(filter_metacharacter(*input, &input), TK_METACHAR);
		else if (is_word(input))
			token->next = new_token(filter_word(input, &input), TK_WORD);
		if (!token->next)
			return (cleanup_token(&(head.next)), NULL);
		token = token->next;
	}
	token->next = new_token(NULL, TK_EOF);
	if (!token->next)
		return (cleanup_token(&(head.next)), NULL);
	token = token->next;
	return (head.next);
}
