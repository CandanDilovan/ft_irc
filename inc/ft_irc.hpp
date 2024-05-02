/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 11:52:41 by dilovan           #+#    #+#             */
/*   Updated: 2024/05/02 11:46:30 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

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
# include <sstream>
# include <ctime>
# include <map>
# include "user_class.hpp"
# include "server_class.hpp"
# include "channel_class.hpp"
# include <stdbool.h>
# include <signal.h>

class SigInted : public std::exception 
{
    public:
    virtual const char *what() const throw()
    {
        return ("Server stopped");
    }
};


#endif 