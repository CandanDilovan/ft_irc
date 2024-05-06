/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelmode_class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 10:50:57 by dilovan           #+#    #+#             */
/*   Updated: 2024/05/06 15:01:28 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/channel_class.hpp"

void    Channel::mode_l(std::string commands)
{
    if (commands.find("-l") != commands.npos)
    {
        _bool_nb_max_of_user = false;
    }
    else if (commands.find("+l") != commands.npos)
    {
        _bool_nb_max_of_user = true;
        std::string user_max = commands.substr((commands.rfind(" ") + 1), commands.find("\r") - (commands.rfind(" ") + 1));
        _nb_max_of_user = atoi(user_max.c_str());
        std::cout << "nb max of user: " << _nb_max_of_user << std::endl;
    }
}

void    Channel::mode_k(std::string commands)
{
    if (commands.find("-k") != commands.npos)
    {
        _pass_on_off = false;
    }
    else if (commands.find("+k") != commands.npos)
    {
        _pass_on_off = true;
        std::string password = commands.substr((commands.rfind(" ") + 1), commands.find("\r") - (commands.rfind(" ") + 1));
        _chan_password = password;
    } 
}

void    Channel::mode_i(std::string commands)
{
    if (commands.rfind("-i") != commands.npos)
            _invit_only = false;
    else if (commands.rfind("+i") != commands.npos)
        _invit_only = true;
}

void    Channel::mode_t(std::string commands)
{
    if (commands.rfind("-t") != commands.npos)
            _modif_topic = false;
    else if (commands.rfind("+t") != commands.npos)
        _modif_topic = true;
}

void    Channel::mode_o(std::string commands, user *users)
{
    if (commands.rfind("-o")!= commands.npos)
	{
		std::string nick = commands.substr((commands.rfind(" ") + 1), commands.find("\r") - (commands.rfind(" ") + 1));
		for (std::list<user *>::iterator it = _oplist.begin(); it != _oplist.end(); it++)
		{
			if ((*it)->getNick() == nick)
			{
                std::string msg = ":" + users->getNick() + " MODE " + _cname  + " -o " + nick + "\r\n";
                sendtoallnopm(msg);
				_oplist.erase(it);
				return ;
			}
		}
        std::string tosend = ":ft_irc 401 " + users->getNick() + " " + nick + " :No such nick/channel" + "\r\n";
        write(users->getFds()->fd, tosend.c_str(), tosend.size());
	}
    else if (commands.rfind("+o")!= commands.npos)
    {
        std::string nick = commands.substr((commands.rfind(" ") + 1), commands.find("\r") - (commands.rfind(" ") + 1));
        if (isop(users) == 1)
        {
            for (std::list<user *>::iterator itt = _ulist.begin(); itt != _ulist.end(); itt++)
            {
                if ((*itt)->getNick() == nick)
                {
                    std::string msg = ":" + users->getNick() + " MODE " + _cname  + " +o " + nick + "\r\n";
                    sendtoallnopm(msg);
                    _oplist.push_back(*itt);
                    return ;
                } 
            }
            std::string tosend = ":ft_irc 441 " + users->getNick() + " " + nick + " " + _cname + " :They aren't on that channel" + "\r\n";
            std::cout << tosend;
            write(users->getFds()->fd, tosend.c_str(), tosend.size());
        }
    }
}
