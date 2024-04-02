/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:37:21 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/02 13:57:55 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstdlib>
#include <poll.h>
#include <string.h>
#include <list>

int main(int argc, char **argv)
{
    (void)argc;
    struct sockaddr_in addr;
    int socket_fd;
    int lene;
    char *error;
    lene = sizeof(addr);
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(socket_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        perror(error);
        std::cerr << "Error: " << error <<  lene << std::endl; 
    }
    listen(socket_fd, 1);
    char tkt[100];
    memset(tkt, 0, 100);
    struct pollfd fds[1];
    fds[0].fd = socket_fd;
    fds[0].events = POLLIN | POLLOUT;
    std::list<struct pollfd *> listfd;
    listfd.push_back(fds);
    while (1)
    {
        for (std::list<struct pollfd *>::iterator it = listfd.begin(); it != listfd.end(); ++it)
        {
            struct pollfd *listfds = *it;
            if (poll(listfds, 1, 0) > 0)
            {
                if (listfds->fd == socket_fd && listfds->revents & POLLIN)
                {
                    struct pollfd *tmp;
                    tmp = new struct pollfd[1];
                    tmp->fd = accept(fds[0].fd, NULL, NULL);
                    if (tmp->fd < 0)
                        perror("error");
                    else
                    {
                        tmp->events = POLLIN | POLLOUT;
                        listfd.push_back(tmp);
                        std::cout << "connexion accepted" << std::endl;
                    }
                }
                else if (listfds->revents & POLLIN)
                {
                    int reading = read(listfds->fd, tkt, 100);
                    if (reading > 0)
                        std::cout << reading << " " << tkt << std::endl;
                    write(listfds->fd, "bien vu\n", 9);
                    memset(tkt, 0, 100);
                }
            }
        }
    }
}
