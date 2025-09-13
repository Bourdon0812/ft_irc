/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yseguin <yseguin@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 15:53:52 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/09/05 12:07:26 by yseguin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "components/Server.hpp"

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	try {
		Server Server(std::atoi(argv[1]), argv[2]);
		Server.run();
	} catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	return 0;
}