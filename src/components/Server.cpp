/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yseguin <yseguin@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:42:19 by yseguin           #+#    #+#             */
/*   Updated: 2025/09/05 12:02:23 by yseguin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "components/Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password), _serverFd(-1) {
	if (port < 1024 || port > 65535) {
		throw std::invalid_argument("Port must be between 1024 and 65535");
	}
	if (password.empty()) {
		throw std::invalid_argument("Password cannot be empty");
	}
}

Server::~Server() {
	close(this->_serverFd);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Server::_setSocket() {
	std::cout << "Setting up socket" << std::endl;
	this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverFd == -1) {
		throw std::runtime_error("Failed to create server socket");
	}

	int yes = 1;
	if (setsockopt(this->_serverFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");
	
	sockaddr_in addr = sockaddr_in();
	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->_port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(this->_serverFd, (sockaddr *)&addr, sizeof(addr)) == -1) {
		throw std::runtime_error("Failed to bind server socket");
	}
	if (listen(this->_serverFd, 10) == -1) {
		throw std::runtime_error("Failed to listen on server socket");
	}

	std::cout << "Socket set up successfully" << std::endl;
}

void Server::_ignoreSigpipe() {
	signal(SIGPIPE, SIG_IGN);
}

void Server::_setNonBlocking(int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		throw std::runtime_error("Failed to get file status flags");
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		throw std::runtime_error("Failed to set file status flags");
	}
}

void Server::_pingAllUsers() {
	long now = Tools::nowMs();
	for (std::map<int, User>::iterator it = this->_users.begin(); it != this->_users.end(); ) {
		User &u = it->second;
		if (u.pingSentMs == 0)
			u.pingSentMs = now;
		if (!u.awaitingPong && now - u.pingSentMs > PING_TIMEOUTMS) {
			u.outBuf += "PING :" + serverHostname + "\r\n";
			u.pingSentMs = now;
			u.awaitingPong = true;
		}
		if (u.awaitingPong && now - u.pingSentMs > PING_TIMEOUTMS) {
			int fd = it->first;
			++it;
			this->_onClientDisconnected(fd);
			continue;
		}
		++it;
	}
}

void Server::_eventLoop() {
	while (true) {
		this->_pingAllUsers();
		fd_set readfds;
		fd_set writefds;
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_SET(this->_serverFd, &readfds);
		int maxFd = this->_serverFd;
		for (std::map<int, User>::iterator it = this->_users.begin(); it != this->_users.end(); it++) {
			FD_SET(it->first, &readfds);
			if (!it->second.outBuf.empty()) {
				FD_SET(it->first, &writefds);
			}
			if (it->first > maxFd) {
				maxFd = it->first;
			}
		}
		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		int activity = select(maxFd + 1, &readfds, &writefds, NULL, &tv);
		if (activity == -1) {
			throw std::runtime_error("select() failed");
		}
		if (FD_ISSET(this->_serverFd, &readfds)) {
			this->_onServerReadable();
		}

		std::vector<int> fdsToRead;
		for (std::map<int, User>::iterator it = this->_users.begin(); it != this->_users.end(); it++) {
			if (FD_ISSET(it->first, &readfds)) {
				fdsToRead.push_back(it->first);
			}
		}
		for (std::vector<int>::iterator it = fdsToRead.begin(); it != fdsToRead.end(); it++) {
			this->_onClientReadable(*it);
		}

		std::vector<int> fdsToWrite;
		for (std::map<int, User>::iterator it = this->_users.begin(); it != this->_users.end(); it++) {
			if (FD_ISSET(it->first, &writefds)) {
				fdsToWrite.push_back(it->first);
			}
		}
		for (std::vector<int>::iterator it = fdsToWrite.begin(); it != fdsToWrite.end(); it++) {
			this->_onClientWritable(*it);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Server::_createLine(std::string &in, std::string &out) {
	std::string::size_type p = in.find("\r\n");
	if (p != std::string::npos) {
		out.assign(in, 0, p);
		in.erase(0, p + 2);
		return true;
	}

	p = in.find('\n');
	if (p != std::string::npos) {
		out.assign(in, 0, p);
		in.erase(0, p + 1);
		return true;
	}

	return false;
}

void Server::_onClientDisconnected(int fd) {
	std::map<int, User>::iterator it = this->_users.find(fd);
	if (it == this->_users.end()) return;
	std::cout << "Client disconnected (fd=" << fd << ")" << std::endl;
	close(fd);
	this->_users.erase(it);
}

void Server::_onServerReadable() {
	int newFd = accept(this->_serverFd, NULL, NULL);
	if (newFd == -1) {
		throw std::runtime_error("Failed to accept new client");
	}
	this->_setNonBlocking(newFd);
	this->_users[newFd] = User();
	this->_users[newFd].lastActivityMs = Tools::nowMs();
	this->_users[newFd].pingSentMs = Tools::nowMs();
	std::cout << "New client connected (fd=" << newFd << ")" << std::endl;
}

void Server::_onClientReadable(int fd) {
	std::map<int, User>::iterator it = this->_users.find(fd);
	if (it == this->_users.end()) return;
	User &u = it->second;
	
	while (true) {
		char buf[4096];
		ssize_t n = recv(fd, buf, sizeof(buf), 0);
		if (n > 0) {
			u.inBuf.append(buf, n);
			u.lastActivityMs = Tools::nowMs();
		} else if (n == 0) {
			this->_onClientDisconnected(fd);
			return;
		} else {
			if (errno == EAGAIN || errno == EWOULDBLOCK) break;
			this->_onClientDisconnected(fd);
			return;
		}
	}
	std::string line;
	while (this->_createLine(u.inBuf, line)) {
		Command command = Parser::parse(line);
		Executer::execute(command, u, *this);
	}
}

void Server::_onClientWritable(int fd) {
	std::map<int, User>::iterator it = this->_users.find(fd);
	if (it == this->_users.end()) return;
	User &u = it->second;
	if (u.outBuf.empty()) return;
	ssize_t n = send(fd, u.outBuf.data(), u.outBuf.size(), 0);
	if (n > 0) {
		u.outBuf.erase(0, static_cast<size_t>(n));
		return;
	}
	if (n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
		return;
	}
	this->_onClientDisconnected(fd);
}
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Server::run() {
	std::cout << "Starting server on port " << this->_port << std::endl;
	this->_setSocket();
	this->_ignoreSigpipe();
	this->_setNonBlocking(this->_serverFd);
	this->_eventLoop();
}

std::string Server::serverHostname = "irc.42.fr";

std::string Server::serverMessage(int code, std::string nick, std::string commandNameFromInput, std::string message) {
	std::string codeStr = Tools::intToString(code);
	while (codeStr.size() < 3) codeStr.insert(codeStr.begin(), '0');
	std::string msg = ":" + Server::serverHostname + " " + codeStr + " " + (nick.empty() ? "*" : nick);
	if (!commandNameFromInput.empty()) {
		msg += " " + commandNameFromInput;
	}
	msg += " :" + message + "\r\n";
	return msg;
}

std::string Server::getPassword() const {
	return this->_password;
}

std::map<int, User>& Server::getUsers() {
	return this->_users;
}

ChannelsManager& Server::getChannelsManager() {
	return this->_channelsManager;
}
