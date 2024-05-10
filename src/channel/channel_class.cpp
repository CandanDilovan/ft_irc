/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:25:46 by dcandan           #+#    #+#             */
/*   Updated: 2024/05/08 14:19:38 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/channel_class.hpp"

Channel::Channel()
{
    
}

Channel::~Channel()
{
    
}

Channel::Channel(user *chuser, std::string cname) : _cname(cname)
{
    _oplist.push_back(chuser);
    _invit_only = false;
    _modif_topic = false;
    _pass_on_off = false;
    _bool_nb_max_of_user = false;
}


void Channel::add_user(user *chuser)
{
    _ulist.push_back(chuser);
    
    std::string joined = ":" + chuser->getNickHost() + " JOIN " + _cname + " " + chuser->getNick() + "\r\n";
    sendtoallnopm(joined);

    std::string joineded = ":ft_irc 353 " + chuser->getNick() + " = " + _cname + " :";
    for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
    {
        if (isop((*it)) == 1)
            joineded += '@' + (*it)->getNick() + " ";
        else
            joineded += (*it)->getNick() + " ";
    }
    joineded += "\r\n:ft_irc 366 " + chuser->getNick() + ' ' + _cname + " :End of /NAMES list\r\n";
    std::cout << joineded;
    write(chuser->getFds()->fd, joineded.c_str(), joineded.size());
}

void Channel::quit_user(user *chuser, std::string str)
{
    for(std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
    {
        if ((*it)->getNick() == chuser->getNick())  
        {
            std::string msg = ":" + chuser->getNickHost() + " QUIT :Quit: " + str + "\r\n";
            sendtoallfr(chuser, msg);
            _ulist.erase(it);
            break;
        }
    }
    rm_op(chuser);
    if (_invit_only == true)
        rm_inv(chuser);
}

void Channel::rm_user(user *chuser, std::string partmsg)
{
    for(std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
    {
        if ((*it)->getNick() == chuser->getNick())  
        {
            std::string msg;

            if (partmsg.size() > 0)
                msg = ":" + chuser->getNickHost() + " PART " + _cname + " " + partmsg + "\r\n";
            else
                msg = ":" + chuser->getNickHost() + " PART " + _cname + "\r\n";
            sendtoallnopm(msg);
            _ulist.erase(it);
            break;
        }
    }
    rm_op(chuser);
    if (_invit_only == true)
        rm_inv(chuser);
}