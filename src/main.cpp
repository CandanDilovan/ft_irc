/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:37:21 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/18 13:57:04 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/user_class.hpp"
#include "../inc/server_class.hpp"
#include "../inc/channel_class.hpp"


void infinite_loop(class Server &serv)
{
    char tkt[100];
    
    while (1)
    {
        memset(tkt, 0, 100);
        for (std::list<class user *>::iterator it = serv.getUserlist().begin(); it != serv.getUserlist().end(); ++it)
        {
            struct pollfd *user_fd = (*it)->getFds();
            if (poll(user_fd, 1, 0) > 0)
            {
                if (user_fd->fd == serv.getUserlist().front()->getFds()->fd && user_fd->revents & POLLIN)
                    serv.add_user();
                else if (user_fd->revents & POLLIN)
                {
                    int reading = read(user_fd->fd, tkt, 100);
                    if (reading > 0)
                        (*it)->allbuff += tkt;
                    std::cout << tkt;
                }
            }
            (*it)->parse_input(serv);
            if ((*it)->allbuff.find("QUIT") != (*it)->allbuff.npos && (*it)->_getco() == 1)
                it = serv.getUserlist().erase(it);
        }
    }
}


int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Error: wrong number of arguments" << std::endl;
        return (1);
    }
    try
    {
        Server serv(argv);
        infinite_loop(serv);
    }
    catch(const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return (0);
}
