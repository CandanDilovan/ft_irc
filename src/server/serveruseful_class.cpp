/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveruseful_class.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dilovan <dilovan@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-04-29 11:38:06 by dilovan           #+#    #+#             */
/*   Updated: 2024-04-29 11:38:06 by dilovan          ###   ########.fr       */
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
    std::list<user *>::iterator it = _userlist.begin();
    std::stringstream convert;
    std::string       replace = puser->getNick();
    int add = 1;
    
    while (it != _userlist.end())
    {
        if ((*it)->getNick() == puser->getNick() && (*it)->getFds() != puser->getFds())
        {
            if (add > 1)
            {
                replace = puser->getNick().substr(0, replace.size());
                convert << add;
                puser->setNick(replace + convert.str());
            }
            else
            {
                convert << add;
                puser->setNick(puser->getNick() + convert.str());
            }
            convert.str(std::string());
            it = _userlist.begin();
            add++;
        }
        it++;
    }
}
