/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dilovan <dilovan@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-04-04 12:59:16 by dilovan           #+#    #+#             */
/*   Updated: 2024-04-04 12:59:16 by dilovan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CLASS_HPP
# define SERVER_CLASS_HPP

# include "ft_irc.hpp"
# include "user_class.hpp"

class Server
{

private :

    std::string             _password;
    std::string             _ports;
    struct sockaddr_in      _addr;
    std::list<user *>       _userlist;

    Server();

public :

    Server(char **argv);
    ~Server();

    std::string             getPass();
    std::string             getPort();
    std::list<user *>       &getUserlist(); 

    void                    add_user();

};

#endif