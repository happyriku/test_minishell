NAME = minishell

SRCS = main.c \
		interpret.c \
		utils.c \
		tokenize/tokenize.c \
		tokenize/new_token.c \
		tokenize/validation.c \
		expand/expand.c \
		parse/parse.c \
		exec/exec.c \
		exec/exec_echo.c \
		redirect/redirect.c \

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