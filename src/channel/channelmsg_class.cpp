/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelmsg_class.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 10:53:26 by dilovan           #+#    #+#             */
/*   Updated: 2024/04/30 10:44:21 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/channel_class.hpp"


void Channel::sendtoallnopm(std::string msg)
{
    for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
        write((*it)->getFds()->fd, msg.c_str(), msg.size());
    //std::cout << msg;
}

void Channel::sendtoallfr(user *chuser, std::string msg)
{
    for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
    {
        if (chuser->getNick() != (*it)->getNick())
            write((*it)->getFds()->fd, msg.c_str(), msg.size());
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
        std::string msg = "Your message cannot be sent (not connected to channel)";
        chuser->error(msg);
    }

}