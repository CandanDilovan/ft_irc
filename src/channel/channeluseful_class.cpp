/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channeluseful_class.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 10:51:53 by dilovan           #+#    #+#             */
/*   Updated: 2024/05/08 14:19:05 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/channel_class.hpp"

int Channel::getUserSize()
{
    return(_ulist.size());
}

std::string Channel::getName()
{
    return (_cname);
}

std::string Channel::getChanPass()
{
    return (_chan_password);
}
void Channel::setChanPass(std::string pass)
{
    _chan_password = pass;
}

bool Channel::invite_on_off()
{
    return(_invit_only);
}

bool Channel::password_on_off()
{
    return (_pass_on_off);
}

bool    Channel::is_in_invite_list(std::string nick)
{
    for (std::list<user *>::iterator it = _invitlist.begin(); it != _invitlist.end(); it++)
    {
        if ((*it)->getNick() == nick)
            return true;
    } 
    return false;
}

int Channel::isinchan(user *chuser)
{
    for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
        if ((*it)->getNick() == chuser->getNick())
            return (1);
    return (0);
}

int Channel::isop(user *chuser)
{
    for (std::list<user *>::iterator it = _oplist.begin(); it != _oplist.end(); it++)
        if ((*it)->getNick() == chuser->getNick())
            return (1);
    return (0);
}

void Channel::notop(user *users)
{
    std::string tosend = ":ft_irc 482 " + users->getNick() + " " + _cname + " :You're not operator on this channel" +"\r\n";
        write(users->getFds()->fd, tosend.c_str(), tosend.size());
}

bool Channel::is_in_op_list(std::string nick)
{
    for (std::list<user *>::iterator it = _oplist.begin(); it != _oplist.end(); it++)
    {
        if ((*it)->getNick() == nick)
            return true;
    }
    return false;
}

void    Channel::rm_op(user *chuser)
{
for(std::list<user *>::iterator it = _oplist.begin(); it != _oplist.end(); it++)
{
    if ((*it)->getNick() == chuser->getNick())
    {
        _oplist.erase(it);
        return ;
    }
}
}

void    Channel::rm_inv(user*chuser)
{
    for(std::list<user *>::iterator it = _invitlist.begin(); it != _invitlist.end(); it++)
    {
        if ((*it)->getNick() == chuser->getNick())
        {
            _invitlist.erase(it);
            return ;
        }
    }
    
}

