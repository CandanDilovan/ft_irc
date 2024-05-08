/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelmode_class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 10:50:57 by dilovan           #+#    #+#             */
/*   Updated: 2024/05/08 11:59:18 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/channel_class.hpp"

void    Channel::mode_l(std::string commands, user *chuser)
{
    if (commands.find("-l") != commands.npos)
    {
        _bool_nb_max_of_user = false;
        std::string tosend = ":" + chuser->getNickHost() + " MODE " + _cname + " -l" + "\r\n";
        sendtoallnopm(tosend);
    }
    else if (commands.find("+l") != commands.npos)
    {
        _bool_nb_max_of_user = true;
        std::string user_max = commands.substr((commands.rfind(" ") + 1), commands.find("\r") - (commands.rfind(" ") + 1));
        if (atoi(user_max.c_str()) > 0)
        {
            _nb_max_of_user = atoi(user_max.c_str());
            std::string tosend = ":" + chuser->getNickHost() + " MODE " + _cname + " +l " + user_max + "\r\n";
            sendtoallnopm(tosend);
        }
        else
        {
            std::string tosend = ":ft_irc 461" + chuser->getNick() + " MODE +l :Not enough parameters\r\n";
            write(chuser->getFds()->fd, tosend.c_str(), tosend.size());
        }
    }
}

void    Channel::mode_k(std::string commands, user *chuser)
{
    if (commands.find("-k") != commands.npos && _chan_password.empty() == false)
    {
        _pass_on_off = false;
        std::string tosend = ":" + chuser->getNickHost() + " MODE " + _cname + " -k " + _chan_password + "\r\n";
        sendtoallnopm(tosend);
        _chan_password.clear();
    }
    else if (commands.find("+k") != commands.npos)
    {
        std::string password = commands.substr((commands.rfind(" ") + 1), commands.find("\r") - (commands.rfind(" ") + 1));
        std::cout << password <<std::endl;
        if (password != "+k")
        {
            _pass_on_off = true;
            _chan_password = password;
            std::string tosend = ":" + chuser->getNickHost() + " MODE " + _cname + " +k " + _chan_password + "\r\n";
            sendtoallnopm(tosend);
        }
    } 
}

void    Channel::mode_i(std::string commands, user *chuser)
{
    if (commands.rfind("-i") != commands.npos)
    {
         _invit_only = false;
        std::string tosend = ":" + chuser->getNickHost() + " MODE " + _cname + " -i\r\n";
        sendtoallnopm(tosend);
    }
    else if (commands.rfind("+i") != commands.npos)
    {
        _invit_only = true;
        std::string tosend = ":" + chuser->getNickHost() + " MODE " + _cname + " +i\r\n";
        sendtoallnopm(tosend);
    }
}

void    Channel::mode_t(std::string commands, user *chuser)
{
    if (commands.rfind("-t") != commands.npos)
    {
        _modif_topic = false;
        std::string tosend = ":" + chuser->getNickHost() + " MODE " + _cname + " -t\r\n";
        sendtoallnopm(tosend);
    }
    else if (commands.rfind("+t") != commands.npos)
    {
        _modif_topic = true;
        std::string tosend = ":" + chuser->getNickHost() + " MODE " + _cname + " +t\r\n";
        sendtoallnopm(tosend);
    }
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
                std::string msg = ":" + users->getNickHost() + " MODE " + _cname  + " -o " + nick + "\r\n";
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
                    std::string msg = ":" + users->getNickHost() + " MODE " + _cname  + " +o " + nick + "\r\n";
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
