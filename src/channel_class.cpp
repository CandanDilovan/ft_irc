/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabel <aabel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:25:46 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/11 12:07:15 by aabel            ###   ########.fr       */
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
    int flag = 0;
    for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
        if ((*it)->getNick() == chuser->getNick())
            flag = 1;
    if (flag == 1)
    {

        for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
        {
            std::string tosend = ":" + chuser->getNick() + " PRIVMSG " + _cname + " :" + msg + "\r\n";
            if ((*it)->getFds()->fd != chuser->getFds()->fd)
                write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
            std::cout << tosend;
        }
    }
    else
    {
        std::string msg = "Your message can't be sent because you've left the channel";
        std::string tosend = ":" + chuser->getNick() + " PRIVMSG " + _cname + " :" + msg + "\r\n";
        write(chuser->getFds()->fd, tosend.c_str(), tosend.size());
    }

}

void Channel::add_user(user *chuser)
{
    _ulist.push_back(chuser);
    std::string joined = "has joined " + _cname;
    sendtoallfr(chuser, joined);
}

void Channel::rm_user(user *chuser)
{
    for(std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
    {
        if ((*it)->getNick() == chuser->getNick())  
        {
            std::string msg = "has left " + _cname;
            sendtoall(chuser, msg);
            msg = "you have left the channel press alt + 1 to return to the main menu ";
            std::string tosend = ":" + chuser->getNick() + " PRIVMSG " + _cname + " :" + msg + "\r\n";
            write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
            _ulist.erase(it);
            break;
        }
    }
}

void Channel::KICK(std::string nick)
{
    std::list<user*>::iterator it = _ulist.begin();
    while (it != _ulist.end())
    {
        if ((*it)->getNick() == nick)
        {
            std::string tosend = ": " + (*it)->getNick() + " KICK " + _cname + " " + nick + "\r\n";
            // write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
            sendtoallfr((*it), tosend);
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

void    TOPIC(std::string topic)
{
    (void) topic;
    // std::string tosend = ":" + getNick() + " INVITE " + nick + " " + _cname + "\r\n";
    // write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
}