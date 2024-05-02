/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelcmd_class.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabel <aabel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 10:51:15 by dilovan           #+#    #+#             */
/*   Updated: 2024/05/02 13:10:07 by aabel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/channel_class.hpp"


void Channel::KICK(user *chuser, std::string nick)
{
    if (isop(chuser) == 1)
    {
        for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
        {
            if ((*it)->getNick() == nick)
            {
                std::string tosend = chuser->getNick() + " KICK " + _cname + " " + nick + "\r\n";
                sendtoallnopm(tosend);
                it = _ulist.erase(it);
                break;
            }
            else
            {
                std::string tosend = chuser->getNick() + " ERROR " + _cname + " :" + "They aren't on that channel" + "\r\n";
                write(chuser->getFds()->fd, tosend.c_str(), tosend.size());
            }
        }
    }
    else
    {
        std::string tosend = chuser->getNick() + " ERROR " + _cname + " :" + "You're not channel operator" + "\r\n";
        write(chuser->getFds()->fd, tosend.c_str(), tosend.size());
    }
}

void Channel::INVITE(std::string nick, std::list<user *> userlist, user *users)
{
    for (std::list<user *>::iterator it = userlist.begin(); it != userlist.end(); it++)
    {
        if ((*it)->getNick() == nick)
        {
            std::string tosend = ":" + users->getNick() + " INVITE " + nick + " " + _cname + "\r\n";
            write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
            _invitlist.push_back(*it);
            break;
        }
        else if (it == _ulist.end())
        {
            std::string tosend = ":" + users->getNick() + " want to invit " + nick + " in the server but not exist" + "\r\n";
            write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
        }
    }
}

void    Channel::TOPIC(std::string topic, user *users)
{
    if (topic.size() != 0 && _modif_topic == true)
        _topic = topic;
    else if (topic.size() != 0 && this->isinchan(users) == 0)
    {
        std::string tosend = users->getNick() + " " + _cname + " :" + "You're not in the channel" + "\r\n";
        write(users->getFds()->fd, tosend.c_str(), tosend.size());
    }
    else if (topic.size() != 0 && _modif_topic == false && this->is_in_op_list(users->getNick()))
        _topic = topic;
    else if (topic.size() != 0 && _modif_topic == false && !this->is_in_op_list(users->getNick()))
    {
        std::string tosend = users->getNick() + " " + _cname + " :" + "You're not channel operator" + "\r\n";
        write(users->getFds()->fd, tosend.c_str(), tosend.size());
    }
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

void    Channel::MODE(std::string commands, user *users)
{
    if (this->is_in_op_list(users->getNick()))
    {
        if (commands.find("+i") != commands.npos || commands.find("-i") != commands.npos)
        {
            mode_i(commands);
        }
        else if (commands.find("-t") != commands.npos || commands.find("+t") != commands.npos)
        {
            mode_t(commands);
        }
        else if ((commands.find("-o") != commands.npos && this->is_in_op_list(users->getNick())) || commands.find("+o") != commands.npos)
        {
            mode_o(commands, users);
        }
        else if (commands.find("-k") != commands.npos || commands.find("+k") != commands.npos)
        {
            mode_k(commands);
        }
        else if (commands.find("-l") != commands.npos || commands.find("+l") != commands.npos)
        {
            mode_l(commands);
        }
    }
    else
    {
        std::string tosend = "Mode not accessible for " + users->getNick() + " because he's not a operator" + "\r\n";
        write(users->getFds()->fd, tosend.c_str(), tosend.size());
    }
        
}

