/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:25:46 by dcandan           #+#    #+#             */
/*   Updated: 2024/05/03 13:19:42 by dcandan          ###   ########.fr       */
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
    
    std::string joined = ":" + chuser->getNick() + " JOIN " + _cname + " " + chuser->getNick() + "\r\n";
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
            for (std::list<user *>::iterator opit = _oplist.begin(); opit != _oplist.end(); opit++)
                if ((*it)->getNick() == chuser->getNick())
                {
                    _oplist.erase(opit);
                    break;
                }
            std::string msg = ":" + chuser->getNick() + " QUIT :Quit: " + str + "\r\n";
            sendtoallfr(chuser, msg);
            _ulist.erase(it);
            break;
        }
    }
}

void Channel::rm_user(user *chuser, std::string partmsg)
{
    for(std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
    {
        if ((*it)->getNick() == chuser->getNick())  
        {
            std::string msg;

            if (partmsg.size() > 0)
                msg = ":" + chuser->getNick() + " PART " + _cname + " " + partmsg + "\r\n";
            else
                msg = ":" + chuser->getNick() + " PART " + _cname + "\r\n";
            sendtoallnopm(msg);
            std::string tosend = ":" + chuser->getNick() + _cname + " :" + msg + "\r\n";
            write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
            _ulist.erase(it);
            break;
        }
    }
    for(std::list<user *>::iterator it = _oplist.begin(); it != _oplist.end(); it++)
    {
        if ((*it)->getNick() == chuser->getNick())
        {
            _oplist.erase(it);
            break;
        }
    }
}