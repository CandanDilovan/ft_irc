/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveruseful_class.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:38:06 by dilovan           #+#    #+#             */
/*   Updated: 2024/04/30 13:12:14 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server_class.hpp"

std::map<std::string, Channel *>    &Server::getMap()
{
    return (_chanmap);
}

std::list<user *> &Server::getUserlist()
{
    return (_userlist);
}

std::string Server::getPass()
{
    return (_password);
}

void Server::twinick(user *puser)
{
    std::list<user *>::iterator     it = _userlist.begin();
    std::stringstream                              convert;
    std::string                 replace = puser->getNick();
    int add = 1;
    
    while (it != _userlist.end())
    {
        if ((*it)->getNick() == puser->getNick() && (*it)->getFds() != puser->getFds())
        {
            convert << add;
            if (add > 1)
            {
                replace = puser->getNick().substr(0, replace.size());
                puser->setNick(replace + convert.str());
            }
            else
                puser->setNick(puser->getNick() + convert.str());
            convert.str(std::string());
            it = _userlist.begin();
            add++;
        }
        it++;
    }
}

void Server::isinchan(user *chuser, std::string msg)
{
    std::list<user *>::iterator it = _userlist.begin();
    for (++it; it != _userlist.end(); it++)
    {
        if (chuser->getNick() != (*it)->getNick())
            write((*it)->getFds()->fd, msg.c_str(), msg.size());
    }
}