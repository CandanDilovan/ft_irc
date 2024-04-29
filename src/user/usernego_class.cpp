/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usernego_class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dilovan <dilovan@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-04-29 11:57:05 by dilovan           #+#    #+#             */
/*   Updated: 2024-04-29 11:57:05 by dilovan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/user_class.hpp"

void user::nego_end(Server &serv)
{
    std::string Willkommen;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char timeloc[100];
    strftime(timeloc, sizeof(timeloc), "%Y/%m/%d %H:%M:%S", ltm);

    Willkommen = "001 " + _nick + " :Welcome to the Internet Relay Network " + _nick + "\r\n";
    write(_fds->fd, Willkommen.c_str(), Willkommen.size());
    Willkommen = "002 " + _nick + " :Your host is ft_irc, running version 1.0" + "\r\n";
    write(_fds->fd, Willkommen.c_str(), Willkommen.size());
    Willkommen = "003 " + _nick + " :This server was created " + timeloc + "\r\n";
    write(_fds->fd, Willkommen.c_str(), Willkommen.size());
    Willkommen = "004 " + _nick + " :There are " + intostr(serv.getUserlist().size() - 1) + " users and " + intostr((serv.getMap().size())) + " Channel on 1 servers" + "\r\n";
    write(_fds->fd, Willkommen.c_str(), Willkommen.size());
    _connected = 1;
    
}

void user::fill_user(std::list<std::string> strings, Server &serv)
{
    int closest;
    
    for(std::list<std::string>::iterator it = strings.begin(); it != strings.end(); ++it)
    {
        if ((*it).find('\r') < (*it).find('\n'))
            closest = (*it).find('\r');
        else
            closest = (*it).find('\n');
        if ((*it).find("CAP LS") != (*it).npos)
            write(_fds->fd, "CAP * LS\n", 9);
        else if ((*it).find("NICK") != (*it).npos)
        {    
            _nick = (*it).substr((*it).find(" ") + 1, closest);
            if (nick_verif() == 1)
            {
                std::string oldnick = _nick;
                _nick = "Guest";
                serv.twinick(this);
                std::string msg = ":" + oldnick + " NICK " + _nick + "\n"  + "\x03" + "8" + "use /Nick <yournick> to change it" + "\x03" + "\r\n";
                write(_fds->fd, msg.c_str(), msg.size());
            }
            else
                serv.twinick(this);
        }
        else  if ((*it).find("USER") != (*it).npos)
            _name = (*it).substr((*it).find(":") + 1, closest);
        else if ((*it).find("PASS") != (*it).npos)
        {
            _upass = (*it).substr((*it).find(" ") + 1, closest);
            if (serv.getPass() != _upass)
            {
                error("Password incorrect");
                usleep(100);
                close(_fds->fd);
            }
        }
        else if ((*it).find("CAP END") != (*it).npos)
        {
            if (_upass.size() > 0)
                nego_end(serv);
            else
            {
                error("Password incorrect");
                usleep(100);
                close(_fds->fd);
            }
            
        }
    }
}
