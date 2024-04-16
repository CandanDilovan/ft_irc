/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_class.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabel <aabel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 12:31:25 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/16 14:50:35 by aabel            ###   ########.fr       */
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
    std::list<user *> _oplist;
    std::list<user *> _invitlist;
    std::string _topic;
    bool _invit_only;
    // bool __modif_topic;
    // int _nb_max_of_user;

    int isop(user *chuser);

public :

    Channel();
    Channel(user *chuser, std::string cname);
    ~Channel();

    int     getUserSize();

    void    add_user(user *chuser);
    void    rm_user(user *chuser);
    bool    invite_on_off();
    bool    is_in_invite_list(std::string nick);
    void    sendtoall(user *chuser, std::string msg);
    void    sendtoallfr(user *chuser, std::string msg);
    void    sendtoallnopm(std::string msg);
    
    void    KICK(user *user, std::string nick);
    void    INVITE(std::string nick, std::list<user *> userlist);
    void    TOPIC(std::string topic, user *users);
    void    MODE(std::string commandes);
};

#endif