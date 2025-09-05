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

#include "Server.hpp"

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

void Server::run() {
	std::cout << "Starting server on port " << this->_port << std::endl;
	this->_setSocket();
}