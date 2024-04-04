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

# include "ft_irc.hpp"

class user
{

private:

    struct pollfd   *_fds;
    std::string     _name;
    std::string     _nick;

    user();

public:

    std::string allbuff;

//constructor

    ~user();
    user(struct pollfd *fds);

//set / get

    struct pollfd   *getFds();

//fonction utile

    void    parse_input();
    void    fill_user(std::list<std::string> strings);


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
    
};


#endif