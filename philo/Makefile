NAME = philo

CC = gcc

OPTIONS = -I .
CFLAGS = -Wall -Werror -Wextra $(OPTIONS)

SRCS = main.c threads.c utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o ./$(NAME) $(OBJS) -lpthread

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
