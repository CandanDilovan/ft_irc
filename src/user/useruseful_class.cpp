/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   useruseful_class.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 12:01:26 by dilovan           #+#    #+#             */
/*   Updated: 2024/05/08 11:58:33 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/user_class.hpp"

std::string user::getNick()
{
    return (_nick);
}

std::string     user::getNickHost()
{
    std::string returning = _nick + "!" + _name + "@localhost";
    return (returning);
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

void user::pinged(Server &serv)
{
    time_t pingus = time(0);

    if (pingus - _sentping > 20 && _pinged == 0)
    {
        std::string ping = "PING ft_irc\r\n";
        write(_fds->fd, ping.c_str(), ping.size());
        _pinged = 1;
    }
    if (pingus - _sentping > 30 && _pinged == 1)
    {
        error("timed out");
        serv.leaveallchan(this, "TIMED OUT");
        close(_fds->fd);
        _connected = 0;
    }
}

void user::waiting_room()
{
    std::string Willkommen;

    Willkommen = "looking up your Hostname, please wait...\r\n";
    
    if (_nick.c_str() && _name.c_str() && _upass.c_str() && _connected == 0)
         write(_fds->fd, Willkommen.c_str(), Willkommen.size());
}

