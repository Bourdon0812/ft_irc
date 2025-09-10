/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:13:45 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/09/10 14:15:19 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User() : passOK(false), nickOk(false), userOk(false), welcome(false), quit(false) {}

/** getter */
bool	User::getPassOK() const
{
	return (passOK);
}

bool	User::getNickOk() const
{
	return (nickOk);
}

bool	User::getUserOk() const
{
	return (userOk);
}

bool	User::getWelcome() const
{
	return (welcome);
}

bool	User::getQuit() const
{
	return (quit);
}

const std::string&	User::getNick() const
{
	return (nick);
}

const std::string&	User::getUsername() const
{
	return (username);
}

const std::string&	User::getRealname() const
{
	return (realname);
}

const std::string&	User::getHostname() const
{
	return (hostname);
}

const std::string&	User::getInBuf() const
{
	return (inBuf);
}

const std::string&	User::getOutBuf() const
{
	return (outBuf);
}

const std::set<std::string>&	User::getChannels() const
{
	return (channels);
}

/** setter */
void	User::setPassOK(bool value)
{
	passOK = value;
}

void	User::setNickOk(bool value)
{
	nickOk = value;
}

void	User::setUserOk(bool value)
{
	userOk = value;
}

void	User::setWelcome(bool value)
{
	welcome = value;
}

void	User::setQuit(bool value)
{
	quit = value;
}

void	User::setNick(const std::string& value)
{
	nick = value;
}

void	User::setUsername(const std::string& value)
{
	username = value;
}

void	User::setRealname(const std::string& value)
{
	realname = value;
}

void	User::setHostname(const std::string& value)
{
	hostname = value;
}

void	User::setInBuf(const std::string& value)
{
	inBuf = value;
}

void	User::setOutBuf(const std::string& value)
{
	outBuf = value;
}

/** methods */
void	User::addChannel(const std::string& channel)
{
	channels.insert(channel);
}

void	User::removeChannel(const std::string& channel)
{
	channels.erase(channel);
}
