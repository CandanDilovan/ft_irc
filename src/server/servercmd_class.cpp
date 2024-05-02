/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servercmd_class.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabel <aabel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:31:15 by dilovan           #+#    #+#             */
/*   Updated: 2024/05/02 13:04:27 by aabel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server_class.hpp"

void    Server::com_spec_kick(user *chuser, std::string line)
{
    if (line.empty())
    {
        std::string tosend = chuser->getNick() + " " + "461" + " " + "INVITE" + " " + "not enough parameters" + "\r\n";
        write(chuser->getFds()->fd, tosend.c_str(), tosend.size());
        return;
    }
    size_t firstSpacePos = line.find(" ");
    std::string cmd = line.substr(0, firstSpacePos);
    
    size_t hashPos = line.find("#", firstSpacePos);
    size_t secondSpacePos = line.find(" ", hashPos);
    std::string chname = line.substr(hashPos, secondSpacePos - hashPos);
    
    std::string nick = line.substr(cmd.size() + chname.size() + 2, line.rfind(" ") - (cmd.size() + chname.size() + 2));
    if (_chanmap.find(chname) != _chanmap.end())
        _chanmap[chname]->KICK(chuser, nick);
    else if (_chanmap.find(chname) == _chanmap.end())
    {
        std::string tosend = chuser->getNick() + " " + chname + " :" + "no such channel" + "\r\n";
        write(chuser->getFds()->fd, tosend.c_str(), tosend.size());
    }
    checkempty(chname);
}

void    Server::com_spec_invite(std::string line, user *users)
{
    if (line.empty())
    {
        std::string tosend = users->getNick() + " " + "461" + " " + "INVITE" + " " + "not enough parameters" + "\r\n";
        write(users->getFds()->fd, tosend.c_str(), tosend.size());
        return;
    }
    size_t firstSpacePos = line.find(" ");
    std::string cmd = line.substr(0, firstSpacePos);
    
    size_t secondSpacePos = line.find(" ", firstSpacePos + 1);
    std::string nick = line.substr(firstSpacePos + 1, secondSpacePos - firstSpacePos - 1);
     
    size_t hashPos = line.find("#", secondSpacePos);
    std::string chname = line.substr(hashPos);
    if (_chanmap.find(chname) != _chanmap.end())
        _chanmap[chname]->INVITE(nick, _userlist, users);
    else if (_chanmap.find(chname) == _chanmap.end())
    {
        std::string tosend = users->getNick() + " " + chname + " :" + "no such channel" + "\r\n";
        write(users->getFds()->fd, tosend.c_str(), tosend.size());
    }
}

void    Server::com_spec_topic(std::string line, user *users)
{
    if (line.empty())
    {
        std::string tosend = users->getNick() + " " + "461" + " " + "TOPIC" + " " + "not enough parameters" + "\r\n";
        write(users->getFds()->fd, tosend.c_str(), tosend.size());
        return;
    }
    size_t firstSpacePos = line.find(" ");
    std::string cmd = line.substr(0, firstSpacePos);
    
    size_t hashPos = line.find("#", firstSpacePos);
    size_t secondSpacePos = line.find(" ", hashPos);
    std::string chname = line.substr(hashPos, secondSpacePos - hashPos);
    
    std::string topic = line.substr(cmd.size() + chname.size() + 3,line.size() - cmd.size() - chname.size());
    if (_chanmap.find(chname) != _chanmap.end())
        _chanmap[chname]->TOPIC(topic, users);
    else if (_chanmap.find(chname) == _chanmap.end())
    {
        std::string tosend = users->getNick() + " " + chname + " :" + "no such channel" + "\r\n";
        write(users->getFds()->fd, tosend.c_str(), tosend.size());
    }
}

void    Server::com_spec_mode(std::string line, user *users)
{
    if (line.find("#") != line.npos)
    {
        size_t firstSpacePos = line.find(" ");
        std::string cmd = line.substr(0, firstSpacePos);
        
        size_t hashPos = line.find("#", firstSpacePos);
        size_t secondSpacePos = line.find(" ", hashPos);
        std::string chname = line.substr(hashPos, secondSpacePos - hashPos);
        
        std::string objectifs = line.substr(cmd.size() + chname.size() + 1, line.size() - cmd.size() - chname.size());
        if (_chanmap.find(chname) != _chanmap.end())
            _chanmap[chname]->MODE(objectifs, users);
    }
    else
        std::cout << "Mode line: " << line << "!" << std::endl;
}
