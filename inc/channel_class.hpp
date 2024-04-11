/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_class.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabel <aabel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 12:31:25 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/11 14:11:40 by aabel            ###   ########.fr       */
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
    std::string _topic;

public :

    Channel();
    Channel(user *chuser, std::string cname);
    ~Channel();

    void    add_user(user *chuser);
    void    rm_user(user *chuser);
    void    sendtoall(user *chuser, std::string msg);
    void    sendtoallfr(user *chuser, std::string msg);
    
    void    KICK(std::string nick);
    void    INVITE(std::string nick);
    void    TOPIC(std::string topic, user *users);
    void    MODE(user *chuser);
};

#endif