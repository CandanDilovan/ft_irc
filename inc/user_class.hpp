/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_class.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabel <aabel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 11:36:35 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/16 12:43:03 by aabel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_CLASS_HPP
# define USER_CLASS_HPP

# include "ft_irc.hpp"

class Server;

class user
{

private:

    int             _connected;
    struct pollfd   *_fds;
    std::string     _name;
    std::string     _nick;
    std::string     _upass;
    std::string     _num;

    user();


    void            ping(Server &serv, std::string str, user *users);
    void            join(Server &serv, std::string str, user *users);
    void            privmsg(Server &serv, std::string str, user *users);
    void            part(Server &serv, std::string str, user *users);
    void            call_spec_comm_kick(Server &serv, std::string str, user *users);
    void            call_spec_comm_invite(Server &serv, std::string str, user *users);
    void            call_spec_comm_topic(Server &Serv, std::string str, user *users);
    void            call_spec_comm_mode(Server &Serv, std::string str, user *users);

public:

    std::string allbuff;

//constructor

    ~user();
    user(struct pollfd *fds, size_t n);

//set / get

    std::string     getNick();
    int             _getco();
    struct pollfd   *getFds();

    void             setNick(std::string newnick);

//fonction utile

    void    parse_input(Server &serv);
    void    fill_user(std::list<std::string> strings, Server &serv);
    void    nego_end();
    void    connected_parse(Server &serv, std::list<std::string> strings);
    void    error(std::string error);


//exception 

    class WrongFds : public std::exception 
    {
        public:
        virtual const char *what() const throw()
        {
            return ("Issue with fds");
        }
    };

    class WrongName : public std::exception 
    {
        public:
        virtual const char *what() const throw()
        {
            return ("Issue with username");
        }
    };

    class ToMany : public std::exception 
    {
        public:
        virtual const char *what() const throw()
        {
            return ("Too many users");
        }
    };

};

#endif