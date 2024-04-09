/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabel <aabel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:06:07 by dilovan           #+#    #+#             */
/*   Updated: 2024/04/09 15:17:36 by aabel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/server_class.hpp"

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

std::list<user *> &Server::getUserlist()
{
    return (_userlist);
}

std::string Server::getPass()
{
    return (_password);
}

void Server::add_user()
{
    struct pollfd   *tmp;
    class user      *newuser;
    
    tmp = new pollfd[1];
    tmp->fd = accept(_userlist.front()->getFds()->fd , NULL, NULL);
    if (tmp->fd < 0)
        perror("error");
    else
    {
        tmp->events = POLLIN | POLLOUT;
        newuser = new user(tmp, _userlist.size());
        _userlist.push_back(newuser);
        std::cout << "connexion accepted" << std::endl;
    }
}

void Server::join_channel(user *chuser, std::string chname)
{
    if (_chanmap.find(chname) == _chanmap.end())
    {
        Channel      *newchan;
        
        newchan = new Channel(chuser, chname);
        _chanmap.insert(std::pair<std::string, Channel *>(chname, newchan));

        std::string joined = "has joined " + chname;
        _chanmap[chname]->sendtoallfr(chuser, joined);
    }
    else
        _chanmap[chname]->add_user(chuser);
}

void Server::tmfm(user *chuser, std::string chname, std::string msg)
{
    _chanmap[chname]->sendtoall(chuser, msg);
}

void    Server::com_spec_kick(std::string line)
{
    if (line.empty())
        return;
    size_t firstSpacePos = line.find(" ");
    if (firstSpacePos == std::string::npos) {
        std::cerr << "Error: command format is incorrect." << std::endl;
        return;
    }
    std::string cmd = line.substr(0, firstSpacePos);
    size_t hashPos = line.find("#");
    size_t SecondSpacePos = line.find(" ");
    if (hashPos == std::string::npos || hashPos < firstSpacePos) {
        std::cerr << "Error: channel name format is incorrect." << std::endl;
        return;
    }
    std::string chname = line.substr(hashPos, SecondSpacePos);
    
    std::string nick = line.substr(cmd.size() + chname.size() + 1, line.rfind(" ") - (cmd.size() + chname.size() + 1));
    std::cout << "cmd:" << cmd << std::endl;
    std::cout << "chname:" << chname << std::endl;
    std::cout << "nick:" << nick << std::endl;
    if (cmd == "KICK")
        _chanmap[chname]->KICK(_userlist.front(), nick);
    // else if (cmd == "TOPIC")
    //     _chanmap[chname]->TOPIC(_userlist.front());
    // else if (cmd == "MODE")
    //     _chanmap[chname]->MODE(_userlist.front());
}

void    Server::com_spec_invite(std::string line)
{
    if (line.empty())
        return;
    size_t firstSpacePos = line.find(" ");
    if (firstSpacePos == std::string::npos) {
        std::cerr << "Error: command format is incorrect." << std::endl;
        return;
    }
    std::string cmd = line.substr(0, firstSpacePos);
    size_t hashPos = line.find("#");
    size_t SecondSpacePos = line.find(" ");
    if (hashPos == std::string::npos || hashPos < firstSpacePos) {
        std::cerr << "Error: channel name format is incorrect." << std::endl;
        return;
    }
    std::string chname = line.substr(hashPos, SecondSpacePos);
    
    std::string nick = line.substr(cmd.size() + chname.size() + 1, line.rfind(" ") - (cmd.size() + chname.size() + 1));
    std::cout << "cmd:" << cmd << std::endl;
    std::cout << "chname:" << chname << std::endl;
    std::cout << "nick:" << nick << std::endl;
    if (cmd == "INVITE")
        _chanmap[chname]->INVITE(_userlist.front(), nick);
    // else if (cmd == "TOPIC")
    //     _chanmap[chname]->TOPIC(_userlist.front());
    // else if (cmd == "MODE")
    //     _chanmap[chname]->MODE(_userlist.front());
}