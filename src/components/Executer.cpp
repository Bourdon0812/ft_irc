#include "components/Executer.hpp"
#include <cctype>

bool Executer::_requireRegistered(User &user, const std::string &cmdName) {
	if (user.nickOk && user.userOk) return true;
	std::string nick = user.nick.empty() ? "*" : user.nick;
	user.outBuf += Server::serverMessage(ERR_NOTREGISTERED, nick, cmdName, "You must be fully connected to use this command.");
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Executer::execute(Command& command, User &user, Server &server) {
	switch (command.type) {
		case PASS:
			_executePass(command, user, server);
			break;
		case NICK:
			_executeNick(command, user, server);
			break;
		case USER:
			_executeUser(command, user);
			break;
		case JOIN:
			_executeJoin(command, user);
			break;
		case PRIVMSG:
			_executePrivmsg(command, user);
			break;
		case KICK:
			_executeKick(command, user);
			break;
		case INVITE:
			_executeInvite(command, user);
			break;
		case TOPIC:
			_executeTopic(command, user);
			break;
		case MODE:
			_executeMode(command, user);
			break;
		case PING:
			_executePing(command, user);
			break;
		case PONG:
			_executePong(command, user);
			break;
		case UNKNOWN:
			_executeUnknown(command, user);
			break;
		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Executer::_executePass(Command& command, User &user, Server &server) {
	std::cout << "Executing PASS command" << std::endl;
	if (user.passOK) {
		user.outBuf += Server::serverMessage(ERR_ALREADYREGISTRED, user.nick, "PASS", "You are already registered");
		return;
	}
	if (command.args.size() == 1 && command.args[0] == server.getPassword()) {
		user.passOK = true;
		user.outBuf += Server::serverMessage(RPL_YOUREOPER, user.nick, "PASS", "You are now registered");
	} else {
		user.outBuf += Server::serverMessage(ERR_PASSWDMISMATCH, user.nick, "PASS", "Password incorrect");
	}
}

void Executer::_executeNick(Command& command, User &user, Server &server) {
	std::cout << "Executing NICK command" << std::endl;
	if (!user.passOK) {
		user.outBuf += Server::serverMessage(ERR_NOTREGISTERED, user.nick, "NICK", "You must be fully connected to use this command.");
		return ;
	}
	if (command.args.size() < 1) {
		user.outBuf += Server::serverMessage(ERR_NEEDMOREPARAMS, user.nick, "NICK", "Not enough parameters");
		return;
	}
	const std::string &newNick = command.args[0];
	if (newNick.empty() || !std::isalpha(static_cast<unsigned char>(newNick[0]))) {
		user.outBuf += Server::serverMessage(ERR_ERRONEUSNICKNAME, user.nick, "NICK", "Nickname is invalid");
		return;
	}
	for (size_t i = 0; i < newNick.size(); ++i) {
		if (!std::isalnum(static_cast<unsigned char>(newNick[i]))) {
			user.outBuf += Server::serverMessage(ERR_ERRONEUSNICKNAME, user.nick, "NICK", "Nickname is invalid");
			return;
		}
	}
	std::map<int, User> temp = server.getUsers();
	for (std::map<int, User>::iterator it = temp.begin(); it != temp.end(); it++) {
		if (it->second.nick == newNick) {
			user.outBuf += Server::serverMessage(ERR_NICKNAMEINUSE, user.nick, "NICK", "Nickname is already in use");
			return ;
		}
	}
	user.nick = newNick;
	user.nickOk = true;
	_checkIfLog(user);
}

void Executer::_executeUser(Command& command, User &user) {
	std::cout << "Executing USER command" << std::endl;
	if (!user.passOK) {
		user.outBuf += Server::serverMessage(ERR_NOTREGISTERED, user.nick, "USER", "You must be fully connected to use this command.");
		return ;
	}
	if (user.nickOk && user.userOk) {
		user.outBuf += Server::serverMessage(ERR_ALREADYREGISTRED, user.nick.empty() ? "*" : user.nick, "USER", "You may not reregister");
	}
	if (command.args.size() != 4) {
		user.outBuf += Server::serverMessage(ERR_NEEDMOREPARAMS, user.nick, "USER", "Not enough parameters");
		return;
	}
	user.username = command.args[0];
	user.realname = command.args[3];
	user.userOk = true;
	_checkIfLog(user);
}

void Executer::_executeJoin(Command& command, User &user) {
	if (!_requireRegistered(user, "JOIN")) return;
	(void)command;
	std::cout << "Executing JOIN command" << std::endl;
}

void Executer::_executePrivmsg(Command& command, User &user) {
	if (!_requireRegistered(user, "PRIVMSG")) return;
	(void)command;
	std::cout << "Executing PRIVMSG command" << std::endl;
}

void Executer::_executeKick(Command& command, User &user) {
	if (!_requireRegistered(user, "KICK")) return;
	(void)command;
	std::cout << "Executing KICK command" << std::endl;
}

void Executer::_executeInvite(Command& command, User &user) {
	if (!_requireRegistered(user, "INVITE")) return;
	(void)command;
	std::cout << "Executing INVITE command" << std::endl;
}

void Executer::_executeTopic(Command& command, User &user) {
	if (!_requireRegistered(user, "TOPIC")) return;
	(void)command;
	std::cout << "Executing TOPIC command" << std::endl;
}

void Executer::_executeMode(Command& command, User &user) {
	if (!_requireRegistered(user, "MODE")) return;
	(void)command;
	std::cout << "Executing MODE command" << std::endl;
}

void Executer::_executePing(Command& command, User &user) {
	(void)command;
	(void)user;
	std::cout << "Executing PING command" << std::endl;
}

void Executer::_executePong(Command& command, User &user) {
	(void)command;
	(void)user;
	std::cout << "Executing PONG command" << std::endl;
}

void Executer::_executeUnknown(Command& command, User &user) {
	std::string reply;
	std::string nick = user.nick.empty() ? "*" : user.nick;

	std::string commandNameFromInput;
	size_t firstSpace = command.originalInput.find(' ');
	if (firstSpace != std::string::npos) {
		commandNameFromInput = command.originalInput.substr(0, firstSpace);
	} else {
		commandNameFromInput = command.originalInput;
	}
	
	size_t first = commandNameFromInput.find_first_not_of(" \t\n\r");
	if (std::string::npos == first) {
		commandNameFromInput = "";
	} else {
		size_t last = commandNameFromInput.find_last_not_of(" \t\n\r");
		commandNameFromInput = commandNameFromInput.substr(first, (last - first + 1));
	}
	if (commandNameFromInput.empty()) {
		commandNameFromInput = "UNKNOWN_COMMAND";
	}

	reply = Server::serverMessage(ERR_UNKNOWNCOMMAND, nick, commandNameFromInput, "Unknown command");
	
	user.outBuf += reply;
}

void Executer::_checkIfLog(User &user) {
	if (user.passOK && user.nickOk && user.userOk && !user.welcome) {
		user.outBuf = Server::serverMessage(RPL_WELCOME, user.nick, "", "Your host is irc.42.fr, running version 1.0");
		user.welcome = true;
	}
}