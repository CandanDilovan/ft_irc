/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabel <aabel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:37:21 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/09 13:46:32 by aabel            ###   ########.fr       */
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
            if ((*it)->_getco() == 0)
                (*it)->parse_input();
            else if ((*it)->allbuff.find("PING") != (*it)->allbuff.npos && (*it)->_getco() == 1)
            {
                std::string ping = (*it)->allbuff.substr((*it)->allbuff.find("PING"),  (*it)->allbuff.rfind('\r') - (*it)->allbuff.find("PING"));
                ping = ping.substr(ping.find(" ") + 1,  ping.rfind('\r') - ping.find(" "));
                ping = "PONG : ft_irc " + ping + "\r\n";
                write((*it)->getFds()->fd, ping.c_str(), ping.size());
                (*it)->allbuff.clear();
            }
            else if ((*it)->allbuff.find("JOIN") != (*it)->allbuff.npos && (*it)->_getco() == 1)
            {
                std::string chname = (*it)->allbuff.substr((*it)->allbuff.find("#"),  (*it)->allbuff.rfind('\r') - (*it)->allbuff.find("#"));
                serv.join_channel(*it, chname);
                (*it)->allbuff.clear();
            }
            else if ((*it)->_getco() == 1 && (*it)->allbuff.find("PRIVMSG") != (*it)->allbuff.npos)
            {
                std::string chname = (*it)->allbuff.substr((*it)->allbuff.find('#'), (*it)->allbuff.find(':') - (*it)->allbuff.find('#') - 1);
                std::string input =  (*it)->allbuff.substr((*it)->allbuff.find(':') + 1, (*it)->allbuff.find('\n') - (*it)->allbuff.find(':'));
                serv.tmfm((*it), chname, input);
                (*it)->allbuff.clear();
            }
            else if (("KICK" == (*it)->allbuff.substr(0, 4) || "INVITE" == (*it)->allbuff.substr(0, 6)))
            {
                serv.com_spec((*it)->allbuff);
                (*it)->allbuff.clear();
            }
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
