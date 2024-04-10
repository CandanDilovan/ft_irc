/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabel <aabel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:25:46 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/10 14:30:50 by aabel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/channel_class.hpp"

Channel::Channel()
{
    
}

Channel::~Channel()
{
    
}

Channel::Channel(user *chuser, std::string cname) : _cname(cname)
{
    _ulist.push_back(chuser);
}

void Channel::sendtoallfr(user *chuser, std::string msg)
{
    for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
    {
        std::string tosend = ":" + chuser->getNick() + " PRIVMSG " + _cname + " :" + msg + "\r\n";
        write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
        std::cout << tosend;
    }

}

void Channel::sendtoall(user *chuser, std::string msg)
{
    for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
    {
        std::string tosend = ":" + chuser->getNick() + " PRIVMSG " + _cname + " :" + msg + "\r\n";
        if ((*it)->getFds()->fd != chuser->getFds()->fd)
            write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
        std::cout << tosend;
    }

}

void Channel::add_user(user *chuser)
{
    _ulist.push_back(chuser);
    std::string joined = "has joined " + _cname;
    sendtoallfr(chuser, joined);
}

void Channel::KICK(std::string nick)
{
    std::list<user*>::iterator it = _ulist.begin();
    while (it != _ulist.end())
    {
        if ((*it)->getNick() == nick)
        {
            std::string tosend = ":" + (*it)->getNick() + " KICK " + _cname + " " + nick + "\r\n";
            write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
            it = _ulist.erase(it);
            break;
        }
        else
        {
            ++it;
        }
    }
}

void Channel::INVITE(std::string nick)
{
    for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
    {
        if ((*it)->getNick() == nick)
        {
            std::string tosend = ": Allready in the Channel " + nick + " " + _cname + "\r\n";
            write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
            break;
        }
        else if (it == _ulist.end())
        {
            std::string tosend = ":" + (*it)->getNick() + " INVITE " + nick + " " + _cname + "\r\n";
            write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
        }
    }
}

