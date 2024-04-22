/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_class.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabel <aabel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 11:45:00 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/22 15:16:07 by aabel            ###   ########.fr       */
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

void user::setNick(std::string newnick)
{
    _nick = newnick;
}

void user::connected_parse(Server &serv, std::list<std::string> strings)
{
	std::string	msg[9] = {"JOIN", "PING", "PRIVMSG", "KICK", "INVITE", "PART", "TOPIC", "MODE", "QUIT"};
	void		(user::*user_list[9])(Server &serv, std::string str, user *users) = {&user::join, &user::ping, &user::privmsg, 
                &user::call_spec_comm_kick, &user::call_spec_comm_invite, &user::part, &user::call_spec_comm_topic, &user::call_spec_comm_mode,
                &user::quit};
	int 		a;

	a = -1;
	while (++a < 9)
	{
        for(std::list<std::string>::iterator it = strings.begin(); it != strings.end(); it++)
        {
            if ((*it).find(msg[a]) != (*it).npos)
            {
                (this->*user_list[a])(serv, (*it), this);
                return ;
            }
        }
	}
}

void    user::quit(Server &serv, std::string str, user *users)
{
    (void) users;
    int cut = str.find(':');
    std::string cutstr = str.substr(cut + 1, (str.find('\r') - cut));
    serv.quit(this, cutstr);
    _connected = 0;
}

void    user::privmsg(Server &serv, std::string str, user *users)
{
    (void) users;
    std::string chname;
    if (str.find('#') != str.npos)
        chname = str.substr(str.find('#'), (str.find(':') - str.find('#') - 1));
    else
        chname = str.substr(str.find(' '), (str.find(':') - str.find('#') - 1));
    std::string input =  str.substr(str.find(':') + 1, str.find('\n') - str.find(':'));
    serv.tmfm(this, chname, input);
}

void    user::join(Server &serv, std::string str, user *users)
{
    (void) users;
    // std::cout << "Join line:" << str << "!" << std::endl; 
    std::string chname = str.substr(str.find("#"),  str.rfind('\r') - str.find("#"));
    // if (str.)
    //     this->_commands_more = 
    serv.join_channel(this, chname);
}

void    user::part(Server &serv, std::string str, user *users)
{
    (void) users;
    std::string chname = str.substr(str.find('#'), (str.find(':') - str.find('#') - 1));
    serv.leaving(this, chname);
}

void    user::ping(Server &serv, std::string str, user *users)
{
    (void)serv;
    (void) users;
    std::string ping = str.substr(str.find("PING"),  (str.rfind('\r') - str.find("PING")));
    ping = ping.substr(ping.find(" ") + 1,  ping.rfind('\r') - ping.find(" "));
    ping = "PONG :ft_irc " + ping + "\r\n";
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
    write(_fds->fd, Willkommen.c_str(), Willkommen.size());
    Willkommen = "002 " + _nick + " :Your host is ft_irc, running version 1.0" + "\r\n";
    write(_fds->fd, Willkommen.c_str(), Willkommen.size());
    Willkommen = "003 " + _nick + " :This server was created " + timeloc + "\r\n";
    write(_fds->fd, Willkommen.c_str(), Willkommen.size());
    Willkommen = "004 " + _nick + " :There are 1 users and 0 services on 1 servers" + "\r\n";
    write(_fds->fd, Willkommen.c_str(), Willkommen.size());
    _connected = 1;
    
}

void user::error(std::string eerror)
{
    std::string error = "ERROR :" + eerror + "\r\n";
    write(_fds->fd, error.c_str(), error.size());
}

void user::waiting_room()
{
    std::string Willkommen;

    Willkommen = "looking up your Hostname, please wait...\r\n";
    
    if (_nick.c_str() && _name.c_str() && _upass.c_str() && _connected == 0)
         write(_fds->fd, Willkommen.c_str(), Willkommen.size());
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
                nego_end();
            else
            {
                error("Password incorrect");
                usleep(100);
                close(_fds->fd);
            }
            
        }
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
            strings.push_back(tmp);
            allbuff.erase(0, closest + 1);
        }
        if (_connected == 0)
            fill_user(strings, serv);
        else if (_connected == 1)
            connected_parse(serv, strings);
        allbuff.clear();
        strings.clear();
    }
        
}

void    user::call_spec_comm_kick(Server &serv, std::string str, user *users)
{
    (void)users;
    serv.com_spec_kick(this, str);
}

void    user::call_spec_comm_invite(Server &serv, std::string str, user *users)
{
    serv.com_spec_invite(str, users);
}

void	user::call_spec_comm_topic(Server &serv, std::string str, user *users)
{
	serv.com_spec_topic(str, users);
}

void    user::call_spec_comm_mode(Server &serv, std::string str, user *users)
{
    // (void) users;
    // (void) str;
    // (void) serv;
    serv.com_spec_mode(str, users);
}