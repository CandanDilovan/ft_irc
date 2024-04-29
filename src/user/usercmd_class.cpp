/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usercmd_class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dilovan <dilovan@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-04-29 11:54:51 by dilovan           #+#    #+#             */
/*   Updated: 2024-04-29 11:54:51 by dilovan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/user_class.hpp"



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
        chname = str.substr(str.find(' ') + 1, (str.find(':') - str.find(' ')) - 2);
    std::string input =  str.substr(str.find(':') + 1, str.find('\n') - str.find(':'));
    serv.tmfm(this, chname, input);
}

void    user::join(Server &serv, std::string str, user *users)
{
    (void) users;
    std::string chname = str.substr(str.find("#"),  str.rfind(" ") - str.find("#"));
    std::string password = str.substr(str.rfind(" ") + 1, str.rfind("\r") - str.rfind(" "));
    if (password != chname)
        this->_commands_more = password;
    serv.join_channel(this, chname);
}

void    user::part(Server &serv, std::string str, user *users)
{
    (void) users;
    serv.leaving(this, str);
}

void    user::ping(Server &serv, std::string str, user *users)
{
    (void)serv;
    (void) users;
    std::string ping = str.substr(str.find("PING"),  (str.rfind('\r') - str.find("PING")));
    ping = ping.substr(ping.find(" ") + 1,  ping.rfind('\r') - ping.find(" "));
    ping = "PONG ft_irc " + ping + "\r\n";
    std::cout << ping << std::endl;
    write(getFds()->fd, ping.c_str(), ping.size());
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
    serv.com_spec_mode(str, users);
}