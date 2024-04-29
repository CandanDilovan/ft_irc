/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userparse_class.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dilovan <dilovan@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-04-29 11:57:54 by dilovan           #+#    #+#             */
/*   Updated: 2024-04-29 11:57:54 by dilovan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/user_class.hpp"

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