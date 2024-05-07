/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelcmd_class.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 10:51:15 by dilovan           #+#    #+#             */
/*   Updated: 2024/05/07 11:50:25 by dcandan          ###   ########.fr       */
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
                std::string tosend = ":" + chuser->getNick() + " KICK " + _cname + " " + nick + "\r\n";
                sendtoallnopm(tosend);
                it = _ulist.erase(it);
                break;
            }
            else
            {
                std::string tosend = ":ft_irc 401 " + chuser->getNick() + " " + nick + " " + " :No such nick/channel" + "\r\n";
                write(chuser->getFds()->fd, tosend.c_str(), tosend.size());
            }
        }
    }
    else
        notop(chuser);
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
            std::string tosend = ":ft_irc 401 " + users->getNick() + " " + nick + " " + " :No such nick/channel" + "\r\n";
            write(users->getFds()->fd, tosend.c_str(), tosend.size());
        }
    }
}

void    Channel::TOPIC(std::string topic, user *users)
{
    if (topic.size() != 0 && _modif_topic == false)
        _topic = topic;
    else if (topic.size() != 0 && this->isinchan(users) == 0)
    {
        std::string tosend = ":ft_irc 442 " + users->getNick() + " " + _cname + " :You're not on that channel" +"\r\n";
        write(users->getFds()->fd, tosend.c_str(), tosend.size());
    }
    else if (topic.size() != 0 && _modif_topic == true && isop(users) == 1)
        _topic = topic;
    else if (topic.size() != 0 && _modif_topic == true && isop(users) == 0)
    {
        notop(users);
        return;
    }
    std::string tosend = "TOPIC " + _cname + " :" + _topic + "\r\n";
    sendtoallnopm(tosend);
}

void    Channel::MODE(std::string commands, user *users)
{
    if (isop(users) == 1)
    {
        if (commands.find("+i") != commands.npos || commands.find("-i") != commands.npos)
            mode_i(commands, users);
        else if (commands.find("-t") != commands.npos || commands.find("+t") != commands.npos)
            mode_t(commands, users);
        else if ((commands.find("-o") != commands.npos && this->is_in_op_list(users->getNick())) || (commands.find("+o") != commands.npos && this->is_in_op_list(users->getNick())))
            mode_o(commands, users);
        else if (commands.find("-k") != commands.npos || commands.find("+k") != commands.npos)
            mode_k(commands, users);
        else if (commands.find("-l") != commands.npos || commands.find("+l") != commands.npos)
            mode_l(commands, users);
        else
        {
            std::string tosend = ":ft_irc 501 " + users->getNick() + " " + _cname + " :Unknown MODE flag" + "\r\n";
            write(users->getFds()->fd, tosend.c_str(), tosend.size());
        }
    }
    else
        notop(users);
        
}

