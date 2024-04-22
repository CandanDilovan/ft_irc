# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/27 11:35:15 by dcandan           #+#    #+#              #
#    Updated: 2024/04/22 12:05:07 by dcandan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_irc
SOURCES = src/main.cpp src/user_class.cpp src/server_class.cpp src/channel_class.cpp
			
OBJECTS = $(SOURCES:.cpp=.o)

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3

all: ${NAME}

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

${NAME}: ${OBJECTS}
	$(CC) $(CFLAGS) -o ${NAME} ${OBJECTS}

clean:
	rm -f ${OBJECTS}
	
fclean: clean
	rm -f ${NAME}

re: fclean all