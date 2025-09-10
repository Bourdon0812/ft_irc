/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 15:53:52 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/09/09 21:07:29 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

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