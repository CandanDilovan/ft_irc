/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabel <aabel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:25:46 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/16 11:50:57 by aabel            ###   ########.fr       */
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
    _oplist.push_back(chuser);
}

int Channel::isop(user *chuser)
{
    for (std::list<user *>::iterator it = _oplist.begin(); it != _oplist.end(); it++)
        if ((*it)->getNick() == chuser->getNick())
            return (1);
    return (0);
}

int Channel::getUserSize()
{
    return(_ulist.size());
}

void Channel::sendtoallnopm(std::string msg)
{
    for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
        write((*it)->getFds()->fd, msg.c_str(), msg.size());
    std::cout << msg;
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
    std::string joined = ":" + chuser->getNick() + " JOIN " + _cname + " " + chuser->getNick() + "\r\n";
    sendtoallnopm(joined);
}

void Channel::rm_user(user *chuser)
{
    for(std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
    {
        if ((*it)->getNick() == chuser->getNick())  
        {
            std::string msg = ":" + chuser->getNick() + " PART " + _cname + " " + chuser->getNick() + "\r\n";
            sendtoallnopm(msg);
            msg = "you have left the channel press alt + 1 to return to the main menu ";
            std::string tosend = ":" + chuser->getNick() + " PRIVMSG " + _cname + " :" + msg + "\r\n";
            write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
            _ulist.erase(it);
            break;
        }
    }
}

void Channel::KICK(user *chuser, std::string nick)
{
    if (isop(chuser) == 1)
    {
        for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
        {
            if ((*it)->getNick() == nick)
            {
                std::string tosend = ":" + (*it)->getNick() + " KICK " + _cname + " " + nick + "\r\n";
                // write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
                sendtoallnopm(tosend);
                it = _ulist.erase(it);
                break;
            }
        }
    }
    else
    {
        std::string tosend = ":" + chuser->getNick() + " PRIVMSG " + _cname + " :" + "you don't have the rights to kick" + "\r\n";
        write(chuser->getFds()->fd, tosend.c_str(), tosend.size());
    }
}

// void Channel::INVITE(std::string nick)
// {
//     for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
//     {
//         if ((*it)->getNick() == nick)
//         {
//             std::string tosend = ": Allready in the Channel " + nick + " " + _cname + "\r\n";
//             write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
//             break;
//         }
//         else if (it == _ulist.end())
//         {
//             std::string tosend = ":" + (*it)->getNick() + " INVITE " + nick + " " + _cname + "\r\n";
//             write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
//         }
//     }
// }

void Channel::INVITE(std::string nick, std::list<user *> userlist)
{
    for (std::list<user *>::iterator it = userlist.begin(); it != userlist.end(); it++)
    {
        if ((*it)->getNick() == nick)
        {
            std::string tosend = ":" + (*it)->getNick() + " INVITE " + nick + " " + _cname + "\r\n";
            write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
            break;
        }
        else if (it == _ulist.end())
        {
            std::string tosend = ":" + (*it)->getNick() + " want to invit " + nick + " in the server but not exist" + "\r\n";
            write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
        }
    }
}

void    Channel::TOPIC(std::string topic, user *users)
{
    if (topic.size() != 0)
        _topic = topic;
    else if (topic.size() == 0)
    {
        for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
        {
            if ((*it)->getNick() == users->getNick())
            {
                std::string tosend = "TOPIC " + _cname + " :" + _topic + "\r\n";
                write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
                return ;
            }
        }
    }
    std::string tosend = "TOPIC " + _cname + " :" + _topic + "\r\n";
    sendtoallnopm(tosend);
}