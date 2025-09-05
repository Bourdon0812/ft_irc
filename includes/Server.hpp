#ifndef Server_HPP
#define Server_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class Server {
private:
	int _port;
	std::string _password;
	int _serverFd;
	std::vector<int> clients;

	void _setSocket();
public:
	Server(int port, std::string password);
	~Server();
	void run();
};

#endif