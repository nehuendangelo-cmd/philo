# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nd-angel <nd-angel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/09 14:25:37 by nd-angel          #+#    #+#              #
#    Updated: 2026/02/09 18:30:50 by nd-angel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = gcc

CFLAGS = -Wall -Wextra -Werror

SRCS = main.c  parsing.c

OBJS = $(SRCS:.c=.o)



$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

all: $(NAME)
	
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

PHONY: all clean fclean re