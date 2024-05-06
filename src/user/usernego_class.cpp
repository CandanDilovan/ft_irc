/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usernego_class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:57:05 by dilovan           #+#    #+#             */
/*   Updated: 2024/05/06 12:43:32 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/user_class.hpp"

void user::wrong_pass()
{
    error("Password incorrect");
    usleep(100);
    close(_fds->fd);
}

void user::wrong_nick(Server &serv)
{
    std::string oldnick = _nick;
    _nick = "Guest";
    serv.twinick(this);
    std::string msg = ":" + oldnick + " NICK " + _nick + "\n"  + "\x03" + "12" + "use /Nick <yournick> to change it" + "\x03" + "\r\n";
    write(_fds->fd, msg.c_str(), msg.size());
}

int user::nick_verif()
{
    for (size_t a = 0; a != _nick.size(); a++)
    {
        if ((_nick[a] >= 'a' && _nick[a] <= 'z') || (_nick[a] >= 'A' && _nick[a] <= 'Z')
            || (_nick[a] >= '0' && _nick[a] <= '9') || _nick[a] == '[' || _nick[a] == ']'
            || _nick[a] == '{' || _nick[a] == '}' || _nick[a] == '\\' || _nick[a] == '|')
        {
            if (_nick[0] >= '0' && _nick[0] <= '9')
            {
                error("Nick: No digit as first character");
                return (1);
            }
            else if( _nick.size() > 30)
            {
                error("Nick: nick lenght limited to 30 characters");
                return 1;
            }
        }
        else
            return (1);
    }
    return (0);
}

void user::nego_end(Server &serv)
{
    std::string Willkommen;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char timeloc[100];
    strftime(timeloc, sizeof(timeloc), "%Y/%m/%d %H:%M:%S", ltm);

    if (_nick.empty() == true || _name.empty() == true || _upass.empty() == true || _capls != 1)
    {
        error("Registration: something went wrong");
        _connected = 0;
        close(_fds->fd);
    }
    else
    {
        Willkommen = "001 " + _nick + " :Welcome to the Internet Relay Network " + _nick + "\r\n";
        write(_fds->fd, Willkommen.c_str(), Willkommen.size());
        Willkommen = "002 " + _nick + " :Your host is ft_irc, running version 1.0" + "\r\n";
        write(_fds->fd, Willkommen.c_str(), Willkommen.size());
        Willkommen = "003 " + _nick + " :This server was created " + timeloc + "\r\n";
        write(_fds->fd, Willkommen.c_str(), Willkommen.size());
        Willkommen = "004 " + _nick + " :There are " + intostr(serv.getUserlist().size() - 1) + " users and " + intostr((serv.getMap().size())) + " Channel on 1 servers" + "\r\n";
        write(_fds->fd, Willkommen.c_str(), Willkommen.size());
        _connected = 1;
        _sentping = time(0);
    } 
}

void user::fill_user(std::list<std::string> strings, Server &serv)
{   
    for(std::list<std::string>::iterator it = strings.begin(); it != strings.end(); ++it)
    {
        int closest = findclosest((*it));
        
        if ((*it).find("CAP LS") != (*it).npos)
        {
            write(_fds->fd, "CAP * LS\n", 9);
            _capls = 1;
        }
        else if ((*it).find("NICK") != (*it).npos)
        {    
            _nick = (*it).substr((*it).find(" ") + 1, closest);
            if (nick_verif() == 1)
                wrong_nick(serv);
            else
                serv.twinick(this);
        }
        else  if ((*it).find("USER") != (*it).npos)
            _name = (*it).substr((*it).find(":") + 1, closest);
        else if ((*it).find("PASS") != (*it).npos)
            _upass = (*it).substr((*it).find(" ") + 1, closest);
        else if ((*it).find("CAP END") != (*it).npos)
        {
            if (serv.getPass() != _upass)
                wrong_pass();
            else if (_upass.size() > 0)
                nego_end(serv);
            else
                wrong_pass();
        }
    }
}
