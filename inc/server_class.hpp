/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 12:59:16 by dilovan           #+#    #+#             */
/*   Updated: 2024/05/07 14:25:17 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CLASS_HPP
# define SERVER_CLASS_HPP

# include "ft_irc.hpp"
# include "user_class.hpp"
# include "channel_class.hpp"

class Channel;

class Server
{

private :

    std::string                         _password;
    std::string                         _ports;
    struct sockaddr_in                  _addr;
    std::list<user *>                   _userlist;
    std::map<std::string, Channel *>    _chanmap;

    Server();

public :

    Server(char **argv);
    ~Server();

    std::string                         getPass();
    std::string                         getPort();
    std::map<std::string, Channel *>    &getMap();
    std::list<user *>                   &getUserlist();
    
    

    void                    add_user();
    int                     checkempty(std::string chname);
    void                    twinick(user *user);
    void                    join_channel(user *chuser, std::string chname);
    void                    leaving(user *chuser, std::string str);
    void                    quit(user *chuser, std::string str);
    void                    leaveallchan(user *chuser, std::string str);

    void                    isinchan(user *chuser, std::string msg);
    
    void                    tmfm(user *chuser, std::string chname, std::string msg);
    
    void                    com_spec_kick(user* chuser, std::string line);
    void                    com_spec_invite(std::string line, user *users);
    void                    com_spec_topic(std::string topic, user *users);
    void                    com_spec_mode(std::string line, user *users);
    void                    join_channel_invit_pass(user *chuser, std::string chname);
};

#endif
