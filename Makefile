# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aabel <aabel@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/27 11:35:15 by dcandan           #+#    #+#              #
#    Updated: 2024/04/17 11:34:57 by aabel            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_irc
SOURCES = src/main.cpp src/user_class.cpp src/server_class.cpp src/channel_class.cpp
			
OBJECTS = $(SOURCES:.cpp=.o)

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

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