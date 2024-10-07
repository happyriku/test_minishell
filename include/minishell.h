#ifndef MINISHELL_H
# define MINISHELL_H

# define EXIT_SUCCESS 0
# define EXIT_FAILED 1

# define PATH_MAX 260

# define EXIT -1
# define MALLOC_ERROR -2

# define SINGLEQUOTE 39
# define DOUBLEQUOTE 34

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <signal.h>
# include <fcntl.h>

typedef enum e_kind
{
	TK_WORD,
	TK_METACHAR,
	TK_OPERATOR,
	TK_EOF,
}	t_kind;

typedef enum e_node_kind
{
	ND_SIMPLE_CMD,
	ND_REDIRECT,
	ND_REDIRECT_OUT,
	ND_REDIRECT_IN,
	ND_REDIRECT_APPEND,
	ND_HEREDOC,

}	t_node_kind;

typedef struct s_token
{
	t_kind			kind;
	struct s_token	*next;
	char			*word;
	int				pid;
}	t_token;

typedef struct s_node
{
	t_token			*args;
	struct s_node	*next;
	t_node_kind		kind;
	//redirect
	struct s_node	*redirect;
	char			*word;
	char			*filename;
	int				std_fd;
	int				file_fd;
}	t_node;

typedef struct s_info
{
	bool	syntax_error;

}	t_info;

extern t_info g_info;

int		interpret(char *input);
void	cleanup_token(t_token **token);

//tokenize
t_token	*tokenize(char *input);
t_token	*new_token(char *str, t_kind kind);
bool	is_word(char *str);
bool	is_blank(char c);
bool	is_metacharacter(char c);
bool	is_ctrlop(char	*str);

//expand
void	expand(t_node *node);

//utils                   *minishellが完成したらlibftの方を用いる
char	*ft_strndup(const char *s1, size_t n);
char	*ft_strncpy(char *dst, char *src, int n);
int		ft_lstsize(t_token *lst);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);

//parse
t_node	*parse(t_token *token);

//exec
int	exec(t_node *node);
int	exec_echo(char **argv, t_node *node);
//redirect
//int	redirect(t_node *node);

//error
void	fatal_error(char *msg);

#endif