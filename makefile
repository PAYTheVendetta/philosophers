# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aialonso <aialonso@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/14 17:47:09 by aialonso          #+#    #+#              #
#    Updated: 2026/02/17 18:12:39 by aialonso         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

ADDS = src
INC_PUS = inc

SOURCES = $(addprefix $(ADDS)/,\
			main.c\
			actions.c\
			actions_waiter.c\
			cleanup.c\
			init.c\
			init_threads.c\
			monitor.c\
			parse.c\
			routine.c\
			time.c)

FLA_I = -I $(INC_PUS)

FLAGS = -Wall -Wextra -Werror -g3

CC = cc

OBJECTS= $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(FLAGS) $(FLA_I) $(OBJECTS) -o $(NAME)
	
%.o: %.c
	@$(CC) $(FLAGS) $(FLA_I) -c $< -o $@

clean:
	rm -f $(OBJECTS)

rclean:
	rm -f $(OBJECTS)
	
fclean: clean
	rm -f $(NAME)

re: fclean all rclean

.PHONY: all clean rclean fclean re

.DEFAULT_GOAL=all
