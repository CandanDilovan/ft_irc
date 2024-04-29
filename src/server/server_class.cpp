/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabel <aabel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:06:07 by dilovan           #+#    #+#             */
/*   Updated: 2024/04/25 13:31:18 by aabel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server_class.hpp"

Server::Server(char **argv)
{
    int socket_fd;
    char *error = NULL;
    struct pollfd *fds = new pollfd[1];

    _password = argv[2];
    _addr.sin_port = htons(atoi(argv[1]));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(socket_fd, (struct sockaddr *) &_addr, sizeof(_addr)) < 0)
    {
        std::cerr << "Error: ";
        perror(error);
    }
    listen(socket_fd, 1);
    fds[0].fd = socket_fd;
    fds[0].events = POLLIN | POLLOUT;
    
    user *socket_user = new user(fds, 1);
    _userlist.push_back(socket_user);
}

Server::Server()
{

}

Server::~Server()
{

}

void Server::tmfm(user *chuser, std::string chname, std::string msg)
{
    if (chname.find('#') != chname.npos && _chanmap.find(chname) != _chanmap.end())
        _chanmap[chname]->sendtoall(chuser, msg);
    else
    {
        for (std::list<user *>::iterator it = _userlist.begin(); it != _userlist.end(); it++)
        {
            if ((*it)->getNick() == chname)
            {
                std::string tosend = ":" + chuser->getNick() + " PRIVMSG " + chname + " :" + msg + "\r\n";
                write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
                return ;
            }
        }
        chuser->error("no such nick/channel");
    }
}
