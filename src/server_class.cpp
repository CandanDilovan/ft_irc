/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabel <aabel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:06:07 by dilovan           #+#    #+#             */
/*   Updated: 2024/04/11 14:11:19 by aabel            ###   ########.fr       */
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

void    Server::checkempty(std::string chname)
{
    if (_chanmap[chname]->getUserSize() < 1)
        _chanmap.erase(chname); 
}

void Server::join_channel(user *chuser, std::string chname)
{
    if (_chanmap.find(chname) == _chanmap.end())
    {
        Channel      *newchan;
        
        newchan = new Channel(chuser, chname);
        _chanmap.insert(std::pair<std::string, Channel *>(chname, newchan));
    }
    _chanmap[chname]->add_user(chuser);
}

void Server::leaving(user *chuser, std::string chname)
{
    _chanmap[chname]->rm_user(chuser);
    checkempty(chname);
}

void Server::tmfm(user *chuser, std::string chname, std::string msg)
{
    if (chname.find('#') != chname.npos)
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
        chuser->error("no such nick");
    }
}

void Server::twinick(user *puser)
{
    for(std::list<user *>::iterator it = _userlist.begin(); it != _userlist.end(); it++)
    {
        if ((*it)->getNick() == puser->getNick() && (*it)->getFds() != puser->getFds())
            puser->setNick(puser->getNick() + "_");
    }
}

void    Server::com_spec_kick(user *user,std::string line)
{
    if (line.empty())
        return;
    size_t firstSpacePos = line.find(" ");
    std::string cmd = line.substr(0, firstSpacePos);
    
    size_t hashPos = line.find("#", firstSpacePos);
    size_t secondSpacePos = line.find(" ", hashPos);
    std::string chname = line.substr(hashPos, secondSpacePos - hashPos);
    
    std::string nick = line.substr(cmd.size() + chname.size() + 2, line.rfind(" ") - (cmd.size() + chname.size() + 2));
    if (_chanmap.find(chname) != _chanmap.end())
        _chanmap[chname]->KICK(user, nick);
    checkempty(chname);
}

void    Server::com_spec_invite(std::string line)
{
    if (line.empty())
        return;
    size_t firstSpacePos = line.find(" ");
    std::string cmd = line.substr(0, firstSpacePos);
    
    size_t secondSpacePos = line.find(" ", firstSpacePos + 1);
    std::string nick = line.substr(firstSpacePos + 1, secondSpacePos - firstSpacePos - 1);
     
    size_t hashPos = line.find("#", secondSpacePos);
    std::string chname = line.substr(hashPos);
    if (_chanmap.find(chname) != _chanmap.end())
        _chanmap[chname]->INVITE(nick);
}

void    Server::com_spec_topic(std::string line, user *users)
{
    if (line.empty())
        return;
    size_t firstSpacePos = line.find(" ");
    std::string cmd = line.substr(0, firstSpacePos);
    
    size_t hashPos = line.find("#", firstSpacePos);
    size_t secondSpacePos = line.find(" ", hashPos);
    std::string chname = line.substr(hashPos, secondSpacePos - hashPos);
    
    std::string topic = line.substr(cmd.size() + chname.size() + 2, line.rfind(" ") - (cmd.size() + chname.size() + 2));
    std::cout << cmd << "!" << std::endl;
    std::cout << chname << "!" << std::endl;
    std::cout << topic << "!" << std::endl;
    if (_chanmap.find(chname) != _chanmap.end())
        _chanmap[chname]->TOPIC(topic, users);
}
