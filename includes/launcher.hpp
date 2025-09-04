#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>

class Launcher {
private:
	int _port;
	std::string _password;
public:
	Launcher(int port, std::string password);
	 
	void run();
};

#endif