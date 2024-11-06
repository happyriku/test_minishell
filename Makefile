NAME = minishell

SRCS = main.c \
		interpret.c \
		utils.c \
		cleanup/cleanup_token.c \
		cleanup/cleanup_node.c \
		tokenize/tokenize.c \
		tokenize/new_token.c \
		tokenize/validation.c \
		expand/expand.c \
		expand/expand_variable.c \
		expand/expand_validation.c \
		parse/parse.c \
		parse/parse_redirect.c \
		parse/append.c \
		exec/exec.c \
		exec/exec_redirect.c \
		exec/exec_command.c \
		signal/signal.c \
		signal/signal_handling.c \
		builtin/builtin.c \
		builtin/builtin_echo.c \
		builtin/builtin_pwd.c \
		builtin/builtin_exit.c \
		builtin/builtin_env.c \
		builtin/builtin_export.c \

OBJS = $(SRCS:.c=.o)

LIBS = -lreadline
CC = cc 
#-fsanitize=address -g

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

.c.o:
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re:	fclean all

.PHONY: all clean fclean re test