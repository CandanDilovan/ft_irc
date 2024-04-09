/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_class.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabel <aabel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 12:31:25 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/09 12:04:51 by aabel            ###   ########.fr       */
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
    void    KICK(user *chuser, std::string nick);
    void    INVITE(user *chuser, std::string nick);
    void    TOPIC(user *chuser);
    void    MODE(user *chuser);
};

#endif