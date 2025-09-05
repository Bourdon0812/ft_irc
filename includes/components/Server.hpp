#ifndef Server_HPP
#define Server_HPP

#include "irc.hpp"

// class used to create a server and manage clients
class Server {
private:
	int _port;
	std::string _password;
	int _serverFd;
	std::map<int, User> _users;

	void _setSocket();
	void _ignoreSigpipe();
	void _setNonBlocking(int fd);
	void _eventLoop();
	bool _createLine(std::string &inBuf, std::string &out);
	void _onClientDisconnected(int fd);
	void _onServerReadable();
	void _onClientReadable(int fd);
public:
	Server(int port, std::string password);
	~Server();
	void run();
};

#endif