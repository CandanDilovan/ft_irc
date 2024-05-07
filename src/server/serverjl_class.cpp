/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverjl_class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:31:18 by dilovan           #+#    #+#             */
/*   Updated: 2024/05/07 10:55:23 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server_class.hpp"

void Server::add_user()
{
    struct pollfd   *tmp;
    class user      *newuser;
    
    tmp = new pollfd[1];
    tmp->fd = accept(_userlist.front()->getFds()->fd , NULL, NULL);
    if (tmp->fd < 0)
        perror("error");
    else
    {
        tmp->events = POLLIN | POLLOUT;
        newuser = new user(tmp, _userlist.size());
        _userlist.push_back(newuser);
        std::cout << "connexion accepted" << std::endl;
    }
}

void Server::join_channel(user *chuser, std::string chname)
{
    if (_chanmap.find(chname) == _chanmap.end() || (_chanmap.find(chname) == _chanmap.end() && chuser->_commands_more.size() != 0))
    {
        Channel      *newchan;
        
        newchan = new Channel(chuser, chname);
        _chanmap.insert(std::pair<std::string, Channel *>(chname, newchan));
        _chanmap[chname]->add_user(chuser);
        if (chuser->_commands_more.size() != 0)
        {
            _chanmap[chname]->setChanPass(chuser->_commands_more);
            _chanmap[chname]->_pass_on_off = true;
        }
    }
    else if (_chanmap[chname]->_bool_nb_max_of_user == false || 
            (_chanmap[chname]->_bool_nb_max_of_user == true && (_chanmap[chname]->getUserSize() + 1) <= _chanmap[chname]->_nb_max_of_user))
    {
        join_channel_invit_pass(chuser, chname);
    }
    else
    {
        for (std::list<user *>::iterator it = _userlist.begin(); it != _userlist.end(); it++)
        {
            if ((*it)->getNick() == chuser->getNick())
            {
                std::string tosend = ":ft_irc 471 " + (*it)->getNick() + " " + chname + " :Cannot join channel (+l)\r\n";
                write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
            }
        }
    }
}

void    Server::join_channel_invit_pass(user *chuser, std::string chname)
{
        if (_chanmap[chname]->invite_on_off() == false && (_chanmap[chname]->password_on_off() == false))
            _chanmap[chname]->add_user(chuser);
        else if (_chanmap[chname]->invite_on_off() == true && _chanmap[chname]->is_in_invite_list(chuser->getNick()) == true)
            _chanmap[chname]->add_user(chuser);
        else if (_chanmap[chname]->invite_on_off() == true && _chanmap[chname]->is_in_invite_list(chuser->getNick()) == false)
        {
            for (std::list<user *>::iterator it = _userlist.begin(); it != _userlist.end(); it++)
                {
                    if ((*it)->getNick() == chuser->getNick())
                    {
                        std::string tosend = ":ft_irc 473 " + (*it)->getNick() + " " + chname + " Can't join the channel (+i)\r\n";
                        write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
                    }
                }
        }
        else if (_chanmap[chname]->password_on_off() == true)
        {
            if (_chanmap[chname]->password_on_off() == true && chuser->_commands_more == _chanmap[chname]->getChanPass())
                _chanmap[chname]->add_user(chuser);
            else if (_chanmap[chname]->password_on_off() == true && chuser->_commands_more != _chanmap[chname]->getChanPass())
            {
                for (std::list<user *>::iterator it = _userlist.begin(); it != _userlist.end(); it++)
                {
                    if ((*it)->getNick() == chuser->getNick())
                    {
                        std::string tosend = ":ft_irc 464 " + (*it)->getNick() + " " + chname + " :Password incorrect\r\n";
                        write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
                    }
                }
            }
        }
}

void Server::leaveallchan(user *chuser, std::string str)
{
    for(std::map<std::string, Channel *>::iterator itch = _chanmap.begin(); itch != _chanmap.end(); itch++)
    {
       itch->second->quit_user(chuser, str);
       if (checkempty(itch->second->getName()) == 1)
       {
            delete itch->second;
            _chanmap.erase(itch);
            if (_chanmap.size() == 0)
                break;
            else
                itch = _chanmap.begin();
       }
    }
}

void Server::quit(user *chuser, std::string str)
{
    leaveallchan(chuser, str);
    close(chuser->getFds()->fd);
}

void Server::leaving(user *chuser, std::string str)
{
    std::string partmsg;
    std::string chname = str.substr(str.find('#'), (str.find(':') - str.find('#') - 1));
    if (str.find(":") != str.npos)
        partmsg = str.substr(str.find(':') + 1, (str.find('\r') - str.find(':')));
    _chanmap[chname]->rm_user(chuser, partmsg);
    checkempty(chname);
}

int    Server::checkempty(std::string chname)
{
    if (_chanmap[chname]->getUserSize() < 1)
        return (1);
    return (0);
}
