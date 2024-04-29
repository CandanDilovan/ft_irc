/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelmode_class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dilovan <dilovan@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-04-29 10:50:57 by dilovan           #+#    #+#             */
/*   Updated: 2024-04-29 10:50:57 by dilovan          ###   ########.fr       */
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
    std::cout << "Invit only: " <<_invit_only << std::endl;
}

void    Channel::mode_t(std::string commands)
{
    if (commands.rfind("-t") != commands.npos)
            _modif_topic = false;
    else if (commands.rfind("+t") != commands.npos)
        _modif_topic = true;
    // std::cout << "Modif topic: " << _modif_topic << std::endl;
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
				_oplist.erase(it);
				break;
			}
			else if (it == _oplist.end())
			{
				std::string tosend = "Mode -o can't possible for " + nick + " because he's not a operator" + "\r\n";
                write(users->getFds()->fd, tosend.c_str(), tosend.size());
			}
		}
	}
    else if (commands.rfind("+o")!= commands.npos)
    {
        std::string nick = commands.substr((commands.rfind(" ") + 1), commands.find("\r") - (commands.rfind(" ") + 1));
        if (this->is_in_op_list(nick) == true)
        {
            std::string tosend = "Mode +o can't possible for " + nick + " because he's allready a operator" + "\r\n";
            write(users->getFds()->fd, tosend.c_str(), tosend.size());
        }
        else if (this->is_in_op_list(nick) == false)
        {
            for (std::list<user *>::iterator itt = _ulist.begin(); itt != _ulist.end(); itt++)
            {
                if ((*itt)->getNick() == nick)
                    _oplist.push_back(*itt); 
                else if (itt == _ulist.end())
                {
                    std::string tosend = "Mode +o can't possible for " + nick + " because he'not on the channel" + "\r\n";
                    write(users->getFds()->fd, tosend.c_str(), tosend.size());
                }
            }
        }
    }
}