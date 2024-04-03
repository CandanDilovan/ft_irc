/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_class.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 11:36:35 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/03 14:36:33 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_CLASS_HPP
# define USER_CLASS_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <iostream>
# include <string>
# include <netdb.h>
# include <unistd.h>
# include <netinet/in.h>
# include <stdio.h>
# include <cstdlib>
# include <poll.h>
# include <string.h>
# include <list>

class user
{
private:
    struct pollfd   *_fds;
    std::string     _name;
    std::string     _nick;
    user();
public:
    ~user();
    user(struct pollfd *fds);

    struct pollfd   *getFds();
    void    setName(std::string name);
    void    setNick(std::string nick);

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

    std::string allbuff;
};


#endif