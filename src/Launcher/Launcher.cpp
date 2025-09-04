/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Launcher.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yseguin <yseguin@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:42:19 by yseguin           #+#    #+#             */
/*   Updated: 2025/09/04 14:42:32 by yseguin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launcher.hpp"

Launcher::Launcher(int port, std::string password) : _port(port), _password(password) {
	if (port < 1024 || port > 65535) {
		throw std::invalid_argument("Port must be between 1024 and 65535");
	}
	if (password.empty()) {
		throw std::invalid_argument("Password cannot be empty");
	}
}

void Launcher::run() {
	std::cout << "Launcher running on port " << this->_port << " with password " << this->_password << std::endl;
}