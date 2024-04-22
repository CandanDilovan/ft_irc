/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_class.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabel <aabel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 12:31:25 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/22 14:28:52 by aabel            ###   ########.fr       */
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
    std::string _chan_password;
    std::string _topic;
    std::list<user *> _ulist;
    std::list<user *> _oplist;
    std::list<user *> _invitlist;
    bool _pass_on_off;
    bool _invit_only;
    bool _modif_topic;
    // int _nb_max_of_user;

    int isop(user *chuser);

public :

    Channel();
    Channel(user *chuser, std::string cname);
    ~Channel();

    int     getUserSize();
    std::string getName();

    void    add_user(user *chuser);
    void    rm_user(user *chuser);
    void    quit_user(user *chuser, std::string str);
    bool    invite_on_off();
    bool    password_on_off();
    bool    is_in_invite_list(std::string nick);
    bool    is_in_op_list(std::string nick);
    void    sendtoall(user *chuser, std::string msg);
    void    sendtoallfr(user *chuser, std::string msg);
    void    sendtoallnopm(std::string msg);
    
    void    KICK(user *user, std::string nick);
    void    INVITE(std::string nick, std::list<user *> userlist, user *users);
    void    TOPIC(std::string topic, user *users);
    
    void    MODE(std::string commandes, user *users);
    void    mode_o(std::string commands, user *users);
    void    mode_i(std::string commands);
    void    mode_t(std::string commands);
    void    mode_k(std::string commands);
};

#endif