/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_class.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 11:45:00 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/09 14:05:41 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/user_class.hpp"

user::user(struct pollfd *fds, size_t n) : _connected(0)
{
    if (fds)
        _fds = fds;
    else
        throw WrongFds();
    std::stringstream convert;
    convert << n;
    if (n < 10)
        _num += "00" + convert.str();
    else if (n < 100)
        _num += "0" + convert.str();
    else if (n < 999)
        _num += convert.str();
    else
        throw ToMany();
}

user::user()
{

}

user::~user()
{
    if (_fds)
        delete[] _fds;
}

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

void user::connected_parse(Server &serv, std::list<std::string> strings)
{
	std::string	msg[3] = {"JOIN", "PING", "PRIVMSG"};
	void		(user::*user_list[4])(Server &serv, std::string str) = {&user::join, &user::ping, &user::privmsg};
	int 		a;

	a = -1;
	while (++a < 3)
	{
        for(std::list<std::string>::iterator it = strings.begin(); it != strings.end(); it++)
        {
            if ((*it).find(msg[a]) != (*it).npos)
            {
                (this->*user_list[a])(serv, (*it));
                return ;
            }
        }
	}
}

void    user::privmsg(Server &serv, std::string str)
{
    std::string chname = str.substr(str.find('#'), (str.find(':') - str.find('#') - 1));
    std::string input =  str.substr(str.find(':') + 1, str.find('\n') - str.find(':'));
    serv.tmfm(this, chname, input);
}

void    user::join(Server &serv, std::string str)
{
    std::string chname = str.substr(str.find("#"),  str.rfind('\r') - str.find("#"));
    serv.join_channel(this, chname);
}

void    user::ping(Server &serv, std::string str)
{
    (void)serv;
    std::string ping = str.substr(str.find("PING"),  (str.rfind('\r') - str.find("PING")));
    ping = ping.substr(ping.find(" ") + 1,  ping.rfind('\r') - ping.find(" "));
    ping = "PONG : ft_irc " + ping + "\r\n";
    write(getFds()->fd, ping.c_str(), ping.size());
}

void user::nego_end()
{
    std::string Willkommen;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char timeloc[100];
    strftime(timeloc, sizeof(timeloc), "%Y/%m/%d %H:%M:%S", ltm);

    Willkommen = "001 " + _nick + " :Welcome to the Internet Relay Network " + _nick + "\r\n";
    write(_fds->fd, Willkommen.c_str(), Willkommen.size() + 1);
    Willkommen = "002 " + _nick + " :Your host is ft_irc, running version 1.0" + "\r\n";
    write(_fds->fd, Willkommen.c_str(), Willkommen.size() + 1);
    Willkommen = "003 " + _nick + " :This server was created " + timeloc + "\r\n";
    write(_fds->fd, Willkommen.c_str(), Willkommen.size() + 1);
    Willkommen = "004 " + _nick + " :There are 1 users and 0 services on 1 servers" + "\r\n";
    write(_fds->fd, Willkommen.c_str(), Willkommen.size() + 1);
    _connected = 1;
    
}

void user::fill_user(std::list<std::string> strings)
{
    int closest;
    
    for(std::list<std::string>::iterator it = strings.begin(); it != strings.end(); it++)
    {
        if ((*it).find('\r') < (*it).find('\n'))
            closest = (*it).find('\r');
        else
            closest = (*it).find('\n');
        if ((*it).find("CAP LS") != (*it).npos)
            write(_fds->fd, "CAP * LS\n", 9);
        if ((*it).find("PASS") != (*it).npos)
            _upass = (*it).substr((*it).find(" ") + 1, closest);
        if ((*it).find("NICK") != (*it).npos)
            _nick = (*it).substr((*it).find(" ") + 1, closest);
        if ((*it).find("USER") != (*it).npos)
            _name = (*it).substr((*it).find(":") + 1, closest);
        if ((*it).find("CAP END") != (*it).npos)
            nego_end();
    }
}

void user::parse_input(Server &serv)
{
    std::list<std::string> strings;
    std::string tmp;
    int closest;

    if (allbuff.find('\r') != allbuff.npos || allbuff.find('\n') != allbuff.npos)
    {
        while (allbuff.find('\r') != allbuff.npos || allbuff.find('\n') != allbuff.npos)
        {
            if (allbuff.find('\r') < allbuff.find('\n'))
                closest = allbuff.find('\r');
            else
                closest = allbuff.find('\n');
            tmp = allbuff.substr(0, closest);
            allbuff.erase(0, closest + 1);
            strings.push_back(tmp);
        }
        if (_connected == 0)
            fill_user(strings);
        else if (_connected == 1)
            connected_parse(serv, strings);
        allbuff.clear();
    }
        
}