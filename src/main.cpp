/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcandan <dcandan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:37:21 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/03 15:43:16 by dcandan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/user_class.hpp"

void add_user(std::list<class user *> &userlist)
{
    struct pollfd   *tmp;
    class user      *newuser;
    
    tmp = new pollfd[1];
    tmp->fd = accept(userlist.front()->getFds()->fd , NULL, NULL);
    if (tmp->fd < 0)
        perror("error");
    else
    {
        tmp->events = POLLIN | POLLOUT;
        newuser = new user(tmp);
        userlist.push_back(newuser);
        std::cout << "connexion accepted" << std::endl;
    }
}

void fill_user(class user* user_one, std::list<std::string> strings)
{
    int closest;
    
    for(std::list<std::string>::iterator it = strings.begin(); it != strings.end(); it++)
    {
        if (user_one->allbuff.find('\r') < user_one->allbuff.find('\n'))
            closest = user_one->allbuff.find('\r');
        else
            closest = user_one->allbuff.find('\n');
        if ((*it).find("NICK") != (*it).npos)
        {
            std::string ok = (*it).substr((*it).find(" ") + 1, closest);
            std::cout << ok << std::endl;
            user_one->setNick(ok);
        }
        if ((*it).find("USER") != (*it).npos)
        {
            std::string ok2 = (*it).substr((*it).find(":") + 1, closest);
            std::cout << ok2 << std::endl;
            user_one->setNick(ok2);
        }
    }
}

void parse_input(class user *user_one)
{
    std::list<std::string> strings;
    std::string tmp;
    int closest;

    while (user_one->allbuff.find('\r') != user_one->allbuff.npos || user_one->allbuff.find('\n') != user_one->allbuff.npos)
    {
        if (user_one->allbuff.find('\r') < user_one->allbuff.find('\n'))
            closest = user_one->allbuff.find('\r');
        else
            closest = user_one->allbuff.find('\n');
        tmp = user_one->allbuff.substr(0, closest);
        user_one->allbuff.erase(0, closest + 1);
        strings.push_back(tmp);
        fill_user(user_one, strings);
    }
}

void infinite_loop(std::list<class user *> &userlist)
{
    char tkt[100];
    
    memset(tkt, 0, 100);
    while (1)
    {
        for (std::list<class user *>::iterator it = userlist.begin(); it != userlist.end(); ++it)
        {
            class user *userlisttmp = *it;
            struct pollfd *user_fd = userlisttmp->getFds();
            if (poll(user_fd, 1, 0) > 0)
            {
                if (user_fd->fd == userlist.front()->getFds()->fd && user_fd->revents & POLLIN)
                    add_user(userlist);
                else if (user_fd->revents & POLLIN)
                {
                    int reading = read(user_fd->fd, tkt, 100);
                    if (reading > 0){

                        std::cout << reading << " " << tkt;
                        userlisttmp->allbuff += tkt;
                    }
                    write(user_fd->fd, "bien vu\n", 9);
                    memset(tkt, 0, 100);
                }
            }
            if (userlisttmp->allbuff.find('\n') || userlisttmp->allbuff.find('\r'))
            {
                parse_input(userlisttmp);
                userlisttmp->allbuff.clear();
            }
        }
    }
}



int main(int argc, char **argv)
{
    (void)argc;
    struct sockaddr_in addr;
    int socket_fd;
    int lene;
    char *error;
    lene = sizeof(addr);
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(socket_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        perror(error);
        std::cerr << "Error: " << error <<  lene << std::endl; 
    }
    listen(socket_fd, 1);
    struct pollfd fds[1];
    fds[0].fd = socket_fd;
    fds[0].events = POLLIN | POLLOUT;
    std::list<class user *> userlist;
    class user *socket_user = new user(fds);
    userlist.push_back(socket_user);
    infinite_loop(userlist);
}
