/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_class.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 11:45:00 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/30 14:55:03 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/user_class.hpp"

user::user(struct pollfd *fds, size_t n) : _connected(2), _capls(0), _pinged(0)
{
    if (fds)
        _fds = fds;
    else
        throw WrongFds();
    std::stringstream convert;
    convert << n;
    if (n < 10)
        _num += "00" + convert.str();
    else if (n < 100)
        _num += "0" + convert.str();
    else if (n < 999)
        _num += convert.str();
    else
        throw ToMany();
    _sentping = time(0);
}

user::user()
{

}

user::~user()
{
    if (_fds)
        delete[] _fds;
}