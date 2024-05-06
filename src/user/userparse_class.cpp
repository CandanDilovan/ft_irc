/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userparse_class.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:57:54 by dilovan           #+#    #+#             */
/*   Updated: 2024/05/06 13:48:55 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/user_class.hpp"


void user::connected_parse(Server &serv, std::list<std::string> strings)
{
	std::string	msg[11] = {"JOIN", "PING", "PRIVMSG", "KICK", "INVITE", "PART", "TOPIC", "MODE", "QUIT", "NICK", "PONG"};
	void		(user::*user_list[11])(Server &serv, std::string str, user *users) = {&user::join, &user::ping, &user::privmsg, 
                &user::call_spec_comm_kick, &user::call_spec_comm_invite, &user::part, &user::call_spec_comm_topic, &user::call_spec_comm_mode,
                &user::quit, &user::nick, &user::pong};
	int 		a;

	a = -1;
	while (++a < 11)
	{
        for(std::list<std::string>::iterator it = strings.begin(); it != strings.end(); it++)
        {
            if ((*it).find(msg[a]) != (*it).npos)
            {
                std::string test = (*it).substr((*it).find(" ") + 1, (*it).find('\n') - (*it).find(" "));
                if (test.size() == 0 && a != 10 && a != 8)
                    error("More Arguments needed");
                else
                    (this->*user_list[a])(serv, (*it), this);
                return ;
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
        if (_connected == 2)
            fill_user(strings, serv);
        else if (_connected == 1)
            connected_parse(serv, strings);
        allbuff.clear();
        strings.clear();
    }
}
