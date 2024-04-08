/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_class.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 12:31:25 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/08 14:47:28 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_CLASS_HPP
# define CHANNEL_CLASS_HPP

# include "ft_irc.hpp"
# include "user_class.hpp"

class user;

class Channel
{

private :

    std::string _cname;
    std::list<user *> _ulist;

public :

    Channel();
    Channel(user *chuser, std::string cname);
    ~Channel();

    void    add_user(user *chuser);
    void    sendtoall(user *chuser, std::string msg);
};

#endif