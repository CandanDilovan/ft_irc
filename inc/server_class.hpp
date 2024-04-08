/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 12:59:16 by dilovan           #+#    #+#             */
/*   Updated: 2024/04/08 13:11:53 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CLASS_HPP
# define SERVER_CLASS_HPP

# include "ft_irc.hpp"
# include "user_class.hpp"
# include "channel_class.hpp"

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

    std::string             getPass();
    std::string             getPort();
    std::list<user *>       &getUserlist(); 
    

    void                    add_user();
    void                    join_channel(user *chuser, std::string chaname);
    //void                    KICK(std::string const &name);
    //void                    INVITE(std::string const &name);
    //void                    TOPIC();
    //void                    MODE(std::string const &mode);

};

#endif
