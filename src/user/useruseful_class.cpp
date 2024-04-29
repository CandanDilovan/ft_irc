/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   useruseful_class.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dilovan <dilovan@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-04-29 12:01:26 by dilovan           #+#    #+#             */
/*   Updated: 2024-04-29 12:01:26 by dilovan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/user_class.hpp"

std::string user::getNick()
{
    return (_nick);
}

struct pollfd *user::getFds()
{
    return (_fds);
}

int user::_getco()
{
    return(_connected);
}

void user::setNick(std::string newnick)
{
    _nick = newnick;
}

std::string user::intostr(int machin)
{
    std::stringstream convert;
    convert << machin;
    return (convert.str());
}

void user::error(std::string eerror)
{
    std::string error = "ERROR :" + eerror + "\r\n";
    write(_fds->fd, error.c_str(), error.size());
}

int user::findclosest(std::string str)
{
    if (str.find('\r') < str.find('\n'))
        return (str.find('\r'));
    else
        return (str.find('\n'));
}

void user::waiting_room()
{
    std::string Willkommen;

    Willkommen = "looking up your Hostname, please wait...\r\n";
    
    if (_nick.c_str() && _name.c_str() && _upass.c_str() && _connected == 0)
         write(_fds->fd, Willkommen.c_str(), Willkommen.size());
}

