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
        _fds = fds;
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

void user::setName(std::string name)
{
    if (name.c_str())
        _name = name;
    else
        throw WrongName();
    
}

void user::setNick(std::string nick)
{
    if (nick.c_str())
        _nick = nick;
    else
        throw WrongName();
    
}

struct pollfd *user::getFds()
{
    return (_fds);
}