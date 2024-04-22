/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabel <aabel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:25:46 by dcandan           #+#    #+#             */
/*   Updated: 2024/04/22 13:25:31 by aabel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/channel_class.hpp"

Channel::Channel()
{
    
}

Channel::~Channel()
{
    
}

Channel::Channel(user *chuser, std::string cname) : _cname(cname)
{
    _oplist.push_back(chuser);
    _invit_only = false;
    _modif_topic = false;
}

int Channel::isop(user *chuser)
{
    for (std::list<user *>::iterator it = _oplist.begin(); it != _oplist.end(); it++)
        if ((*it)->getNick() == chuser->getNick())
            return (1);
    return (0);
}

int Channel::getUserSize()
{
    return(_ulist.size());
}

std::string Channel::getName()
{
    return (_cname);
}

bool Channel::invite_on_off()
{
    return(_invit_only);
}

bool    Channel::is_in_invite_list(std::string nick)
{
    bool send;
    for (std::list<user *>::iterator it = _invitlist.begin(); it != _invitlist.end(); it++)
    {
        if ((*it)->getNick() == nick)
        {
            send = true;
            break;
        }
        else if (it == _invitlist.end())
            send = false;
    } 
    return (send);  
}

void Channel::sendtoallnopm(std::string msg)
{
    for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
        write((*it)->getFds()->fd, msg.c_str(), msg.size());
    std::cout << msg;
}

void Channel::sendtoallfr(user *chuser, std::string msg)
{
    for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
    {
        std::string tosend = ":" + chuser->getNick() + " PRIVMSG " + _cname + " :" + msg + "\r\n";
        write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
        std::cout << tosend;
    }
}

void Channel::sendtoall(user *chuser, std::string msg)
{
    int flag = 0;
    for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
        if ((*it)->getNick() == chuser->getNick())
            flag = 1;
    if (flag == 1)
    {

        for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
        {
            std::string tosend = ":" + chuser->getNick() + " PRIVMSG " + _cname + " :" + msg + "\r\n";
            if ((*it)->getFds()->fd != chuser->getFds()->fd)
                write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
            std::cout << tosend;
        }
    }
    else
    {
        std::string msg = "Your message can't be sent because you've left the channel";
        std::string tosend = ":" + chuser->getNick() + " PRIVMSG " + _cname + " :" + msg + "\r\n";
        write(chuser->getFds()->fd, tosend.c_str(), tosend.size());
    }

}

bool Channel::is_in_op_list(std::string nick)
{
    bool    send;
    for (std::list<user *>::iterator it = _oplist.begin(); it != _oplist.end(); it++)
    {
        if ((*it)->getNick() == nick)
        {
            send = true;
            break;
        }
        else if (it == _invitlist.end())
            send = false;
    }
    return (send);
}

void Channel::add_user(user *chuser)
{
    _ulist.push_back(chuser);
    
    std::string joined = ":" + chuser->getNick() + " JOIN " + _cname + " " + chuser->getNick() + "\r\n";
    sendtoallnopm(joined);

    std::string joineded = ":ft_irc 353 " + chuser->getNick() + " = " + _cname + " :";
    for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
    {
        if (isop((*it)) == 1)
            joineded += '@' + (*it)->getNick() + " ";
        else
            joineded += (*it)->getNick() + " ";
    }
    joineded += "\r\n:ft_irc 366 " + chuser->getNick() + ' ' + _cname + " :End of /NAMES list\r\n";
    std::cout << joineded;
    write(chuser->getFds()->fd, joineded.c_str(), joineded.size());
}

void Channel::quit_user(user *chuser, std::string str)
{
    for(std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
    {
        if ((*it)->getNick() == chuser->getNick())  
        {
            for (std::list<user *>::iterator opit = _oplist.begin(); opit != _oplist.end(); opit++)
                if ((*it)->getNick() == chuser->getNick())
                {
                    _oplist.erase(opit);
                    break;
                }
            std::string msg = ":" + chuser->getNick() + " QUIT :Quit: " + str + "\r\n";
            sendtoallnopm(msg);
            _ulist.erase(it);
            break;
        }
    }
}


void Channel::rm_user(user *chuser)
{
    for(std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
    {
        if ((*it)->getNick() == chuser->getNick())  
        {
            std::string msg = ":" + chuser->getNick() + " PART " + _cname + " " + chuser->getNick() + "\r\n";
            sendtoallnopm(msg);
            msg = "you have left the channel press alt + 1 to return to the main menu ";
            std::string tosend = ":" + chuser->getNick() + " PRIVMSG " + _cname + " :" + msg + "\r\n";
            write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
            _ulist.erase(it);
            break;
        }
    }
}

void Channel::KICK(user *chuser, std::string nick)
{
    if (isop(chuser) == 1)
    {
        for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
        {
            if ((*it)->getNick() == nick)
            {
                std::string tosend = ":" + (*it)->getNick() + " KICK " + _cname + " " + nick + "\r\n";
                sendtoallnopm(tosend);
                it = _ulist.erase(it);
                break;
            }
        }
    }
    else
    {
        std::string tosend = ":" + chuser->getNick() + " PRIVMSG " + _cname + " :" + "you don't have the rights to kick" + "\r\n";
        write(chuser->getFds()->fd, tosend.c_str(), tosend.size());
    }
}

void Channel::INVITE(std::string nick, std::list<user *> userlist, user *users)
{
    for (std::list<user *>::iterator it = userlist.begin(); it != userlist.end(); it++)
    {
        if ((*it)->getNick() == nick)
        {
            std::string tosend = ":" + users->getNick() + " INVITE " + nick + " " + _cname + "\r\n";
            write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
            _invitlist.push_back(*it);
            break;
        }
        else if (it == _ulist.end())
        {
            std::string tosend = ":" + users->getNick() + " want to invit " + nick + " in the server but not exist" + "\r\n";
            write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
        }
    }
}

void    Channel::TOPIC(std::string topic, user *users)
{
    if (topic.size() != 0 && _modif_topic == true)
        _topic = topic;
    else if (topic.size() != 0 && _modif_topic == false && this->is_in_op_list(users->getNick()))
        _topic = topic;
    else if (topic.size() == 0)
    {
        for (std::list<user *>::iterator it = _ulist.begin(); it != _ulist.end(); it++)
        {
            if ((*it)->getNick() == users->getNick())
            {
                std::string tosend = "TOPIC " + _cname + " :" + _topic + "\r\n";
                write((*it)->getFds()->fd, tosend.c_str(), tosend.size());
                return ;
            }
        }
    }
    std::string tosend = "TOPIC " + _cname + " :" + _topic + "\r\n";
    sendtoallnopm(tosend);
}

void    Channel::MODE(std::string commands, user *users)
{
    if (commands.rfind("i") != commands.npos)
    {
        (void) users;
        mode_i(commands);
    }
    else if (commands.rfind("t") != commands.npos)
    {
        (void) users;
        mode_t(commands);
    }
    else if ((commands.rfind("-o") != commands.npos && this->is_in_op_list(users->getNick())) || commands.rfind("+o") != commands.npos)
    {
        mode_o(commands, users);
    }
    // else if (commands.find("k") != commands.npos)
    //     std::cout << "k" << std::endl;
    // else if (commands.find("l") != commands.npos)
    //     std::cout << "l" << std::endl;
}

void    Channel::mode_i(std::string commands)
{
    if (commands.rfind("-") != commands.npos)
            _invit_only = false;
    else if (commands.rfind("+") != commands.npos)
        _invit_only = true;
    std::cout << "Invit only: " <<_invit_only << std::endl;
}

void    Channel::mode_t(std::string commands)
{
    if (commands.rfind("-") != commands.npos)
            _modif_topic = false;
    else if (commands.rfind("+") != commands.npos)
        _modif_topic = true;
    std::cout << "Modif topic: " << _modif_topic << std::endl;
}

void    Channel::mode_o(std::string commands, user *users)
{
    if (commands.rfind("-o")!= commands.npos)
	{
		std::string nick = commands.substr((commands.rfind(" ") + 1), commands.find("\r") - (commands.rfind(" ") + 1));
		for (std::list<user *>::iterator it = _oplist.begin(); it != _oplist.end(); it++)
		{
			if ((*it)->getNick() == nick)
			{
				_oplist.erase(it);
				break;
			}
			else if (it == _oplist.end())
			{
				std::string tosend = "Mode -o can't possible for " + nick + " because he's not a operator" + "\r\n";
                write(users->getFds()->fd, tosend.c_str(), tosend.size());
			}
		}
	}
    else if (commands.rfind("+o")!= commands.npos)
    {
        std::string nick = commands.substr((commands.rfind(" ") + 1), commands.find("\r") - (commands.rfind(" ") + 1));
        if (this->is_in_op_list(nick) == true)
        {
            std::string tosend = "Mode +o can't possible for " + nick + " because he's allready a operator" + "\r\n";
            write(users->getFds()->fd, tosend.c_str(), tosend.size());
        }
        else if (this->is_in_op_list(nick) == false)
        {
            for (std::list<user *>::iterator itt = _ulist.begin(); itt != _ulist.end(); itt++)
            {
                if ((*itt)->getNick() == nick)
                    _oplist.push_back(*itt); 
                else if (itt == _ulist.end())
                {
                    std::string tosend = "Mode +o can't possible for " + nick + " because he'not on the channel" + "\r\n";
                    write(users->getFds()->fd, tosend.c_str(), tosend.size());
                }
            }
        }
    }
}