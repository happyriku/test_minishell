#ifndef MINISHELL_H
# define MINISHELL_H

# define EXIT_SUCCESS 0
# define EXIT_FAILED 1

# define PATH_MAX 260

# define MALLOC_ERROR -2

# define SINGLEQUOTE 39
# define DOUBLEQUOTE 34

# define CHILD	"child"
# define PARENT "parent"

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
# include <errno.h>

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
	ND_PIPELINE,

}	t_node_kind;

typedef struct s_token
{
	t_kind			kind;
	struct s_token	*next;
	char			*word;
	//execve
	char			**arr;
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
	char			*delimiter;
	int				std_fd;
	int				file_fd;
	int				stash_fd;
	int				pipefd[2];
	//pipe
	int				pfd[2];
	struct s_node	*pipeline;
}	t_node;

typedef struct s_info
{
	bool			syntax_error;
	bool			fatal_error;
	unsigned int	last_status;
	volatile sig_atomic_t signal;

}	t_info;

extern t_info g_info;

int		interpret(char *input, int *last_status);
void	cleanup_token(t_token *token);
void	cleanup_node(t_node *node);

//tokenize
t_token	*tokenize(char *input);
t_token	*new_token(char *str, t_kind kind);
bool	is_word(char *str);
bool	is_blank(char c);
bool	is_metacharacter(char c);
bool	is_ctrlop(char	*str);

//expand
void	expand(t_node *node);
void	expand_variable(t_token *args);
bool	is_variable(char *word);
bool	is_special_parameter(char *word);
void	syntax_error(void);
void	reset_redirect(t_node *redirect);
char	*remove_quote(char *word);
void	append_char(char **new_word, char word);

//utils                   *minishellが完成したらlibftの方を用いる
char	*ft_strndup(const char *s1, size_t n);
char	*ft_strncpy(char *dst, char *src, int n);
int		ft_lstsize(t_token *lst);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
int		ft_isalpha(int c);

//parse
t_node	*parse(t_token *token);
t_node	*redirect_node(t_token **rest, char *word, t_node_kind kind);
t_node	*heredoc_node(t_token **rest, char *word, t_node_kind kind);
void	append_node(t_node	**redirect, t_node *node);
void	append_token(t_token **args, t_token *token);
bool	has_redirect(t_token *token);
t_token	*tokdup(t_token *token);

//exec
int		exec(t_node *node);
int		exec_echo(char **argv, t_node *node);
int		exec_builtin(t_node *node);
void	exec_nonbuiltin(t_node *node);
void	do_redirect(t_node *redirect);
void	do_heredoc(t_node *redirect);
int		exec_command(t_node *node, int in_fd);
bool	is_builtin(t_token *token);

char	*search_path(char *input);

//signal
void	setup_signal(void);
void	reset_signals(void);

//error
void	fatal_error(char *msg);

#endif