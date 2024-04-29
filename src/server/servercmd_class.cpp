/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servercmd_class.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dilovan <dilovan@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-04-29 11:31:15 by dilovan           #+#    #+#             */
/*   Updated: 2024-04-29 11:31:15 by dilovan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server_class.hpp"

void    Server::com_spec_kick(user *chuser, std::string line)
{
    if (line.empty())
        return;
    size_t firstSpacePos = line.find(" ");
    std::string cmd = line.substr(0, firstSpacePos);
    
    size_t hashPos = line.find("#", firstSpacePos);
    size_t secondSpacePos = line.find(" ", hashPos);
    std::string chname = line.substr(hashPos, secondSpacePos - hashPos);
    
    std::string nick = line.substr(cmd.size() + chname.size() + 2, line.rfind(" ") - (cmd.size() + chname.size() + 2));
    if (_chanmap.find(chname) != _chanmap.end())
        _chanmap[chname]->KICK(chuser, nick);
    checkempty(chname);
}

void    Server::com_spec_invite(std::string line, user *users)
{
    if (line.empty())
        return;
    size_t firstSpacePos = line.find(" ");
    std::string cmd = line.substr(0, firstSpacePos);
    
    size_t secondSpacePos = line.find(" ", firstSpacePos + 1);
    std::string nick = line.substr(firstSpacePos + 1, secondSpacePos - firstSpacePos - 1);
     
    size_t hashPos = line.find("#", secondSpacePos);
    std::string chname = line.substr(hashPos);
    if (_chanmap.find(chname) != _chanmap.end())
        _chanmap[chname]->INVITE(nick, _userlist, users);
}

void    Server::com_spec_topic(std::string line, user *users)
{
    if (line.empty())
        return;
    size_t firstSpacePos = line.find(" ");
    std::string cmd = line.substr(0, firstSpacePos);
    
    size_t hashPos = line.find("#", firstSpacePos);
    size_t secondSpacePos = line.find(" ", hashPos);
    std::string chname = line.substr(hashPos, secondSpacePos - hashPos);
    
    std::string topic = line.substr(cmd.size() + chname.size() + 3,line.size() - cmd.size() - chname.size());
    if (_chanmap.find(chname) != _chanmap.end())
        _chanmap[chname]->TOPIC(topic, users);
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
