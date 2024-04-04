/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_class.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 11:45:00 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/03 13:32:20 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/user_class.hpp"

user::user(struct pollfd *fds)
{
    if (fds)
    {
        _fds = fds;
    }
    else
        throw WrongFds();
}

user::user()
{

}

user::~user()
{
    if (_fds)
        delete[] _fds;
}


struct pollfd *user::getFds()
{
    return (_fds);
}

void user::fill_user(std::list<std::string> strings)
{
    int closest;
    
    for(std::list<std::string>::iterator it = strings.begin(); it != strings.end(); it++)
    {
        if ((*it).find('\r') < (*it).find('\n'))
            closest = (*it).find('\r');
        else
            closest = (*it).find('\n');
        if ((*it).find("NICK") != (*it).npos)
            _nick = (*it).substr((*it).find(" ") + 1, closest);
        if ((*it).find("USER") != (*it).npos)
            _name = (*it).substr((*it).find(":") + 1, closest);
    }
}

void user::parse_input()
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
            allbuff.erase(0, closest + 1);
            strings.push_back(tmp);
            fill_user(strings);
        }
        allbuff.clear();
        std::cout << _name << std::endl;
        std::cout << _nick << std::endl;
    }
}