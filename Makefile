# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/27 11:35:15 by dcandan           #+#    #+#              #
#    Updated: 2024/05/08 13:09:06 by dcandan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
SOURCES = src/main.cpp \
		src/user/user_class.cpp src/user/usercmd_class.cpp src/user/usernego_class.cpp src/user/userparse_class.cpp src/user/useruseful_class.cpp\
		src/server/server_class.cpp src/server/servercmd_class.cpp src/server/serveruseful_class.cpp src/server/serverjl_class.cpp \
		src/channel/channel_class.cpp src/channel/channelcmd_class.cpp src/channel/channelmode_class.cpp src/channel/channelmsg_class.cpp src/channel/channeluseful_class.cpp
			
OBJECTS = $(SOURCES:.cpp=.o)

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

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