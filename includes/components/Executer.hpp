#ifndef EXECUTER_CPP
#define EXECUTER_CPP

#include "irc.hpp"

class Server;

// class used to execute the command
class Executer {
private:
	static bool _requireRegistered(User &user, const std::string &cmdName);

	static void _executePass(Command& command, User &user, Server &server);
	static void _executeNick(Command& command, User &user, Server &server);
	static void _executeUser(Command& command, User &user);
	static void _executeJoin(Command& command, User &user, Server &server);
	static void _executePrivmsg(Command& command, User &user, Server &server);
	static void _executeKick(Command& command, User &user);
	static void _executeInvite(Command& command, User &user);
	static void _executeTopic(Command& command, User &user);
	static void _executeMode(Command& command, User &user);
	static void _executePing(Command& command, User &user);
	static void _executePong(Command& command, User &user);
	static void _executeUnknown(Command& command, User &user);

	static void _checkIfLog(User &user);
public:
	static void execute(Command& command, User &user, Server &server);
};

#endif
