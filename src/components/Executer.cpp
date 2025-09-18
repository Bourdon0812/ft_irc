#include "components/Executer.hpp"
#include "components/Server.hpp"
#include <cctype>

bool Executer::_requireRegistered(User &user, const std::string &cmdName) {
	if (user.nickOk && user.userOk) return true;
	std::string nick = user.nick.empty() ? "*" : user.nick;
	user.outBuf += Server::serverMessage(ERR_NOTREGISTERED, nick, cmdName, "You must be fully connected to use this command.");
	return false;
}

void Executer::_checkIfLog(User &user) {
	if (user.passOK && user.nickOk && user.userOk && !user.welcome) {
		user.outBuf = Server::serverMessage(RPL_WELCOME, user.nick, "", "Your host is irc.42.fr, running version 1.0");
		user.welcome = true;
	}
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
			_executeJoin(command, user, server);
			break;
		case PRIVMSG:
			_executePrivmsg(command, user, server);
			break;
		case KICK:
			_executeKick(command, user, server);
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
	std::map<int, User> &users = server.getUsers();
	for (std::map<int, User>::iterator it = users.begin(); it != users.end(); it++) {
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

void Executer::_executePrivmsg(Command& command, User &user, Server &server) {
	if (!_requireRegistered(user, "PRIVMSG")) return;
	std::cout << "Executing PRIVMSG command" << std::endl;
	if (command.args.size() < 2) {
		user.outBuf += Server::serverMessage(ERR_NEEDMOREPARAMS, user.nick, "PRIVMSG", "Not enough parameters");
		return;
	}
	if (command.args[0].empty() || command.args[1].empty()) {
		user.outBuf += Server::serverMessage(ERR_NEEDMOREPARAMS, user.nick, "PRIVMSG", "Not enough parameters");
		return;
	}
	const std::string &target = command.args[0];
	const std::string &message = command.args[1];
	if (target[0] == '#') {
		ChannelsManager& channelManager = server.getChannelsManager();
		Channel* channel = channelManager.getChannel(target);
		
		if (channel == NULL) {
			user.outBuf += Server::serverMessage(ERR_NOSUCHCHANNEL, user.nick, target, "No such channel");
			return;
		}
		bool userInChannel = false;
		for (std::vector<User*>::iterator it = channel->users.begin(); it != channel->users.end(); ++it) {
			if (*it == &user) {
				userInChannel = true;
				break;
			}
		}
		if (!userInChannel) {
			user.outBuf += Server::serverMessage(ERR_NOTONCHANNEL, user.nick, target, "You're not on that channel");
			return;
		}
		std::string privmsgMessage = ":" + user.nick + " PRIVMSG " + target + " :" + message + "\r\n";
		channelManager.notifyChannel(target, privmsgMessage, &user);
	} else {
		std::map<int, User> &users2 = server.getUsers();
		User* targetUser = Tools::findUserByNick(target, users2);
		if (targetUser == NULL) {
			user.outBuf += Server::serverMessage(ERR_NOSUCHNICK, user.nick, target, "No such nick/channel");
			return;
		}
		targetUser->outBuf += ":" + user.nick + " PRIVMSG " + target + " :" + message + "\r\n";
	}
	user.lastActivityMs = Tools::nowMs();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Executer::_executeJoin(Command& command, User &user, Server &server) {
	std::string password = "";
	if (!_requireRegistered(user, "JOIN")) return;
	std::cout << "Executing JOIN command" << std::endl;
	if (command.args.size() < 1) {
		user.outBuf += Server::serverMessage(ERR_NEEDMOREPARAMS, user.nick, "JOIN", "Not enough parameters");
		return;
	}
	const std::string &channelName = command.args[0];
	if (channelName.empty() || channelName[0] != '#') {
		user.outBuf += Server::serverMessage(ERR_NOSUCHCHANNEL, user.nick, "JOIN", "No such channel");
		return;
	}
	if (command.args.size() > 1) {
		password = command.args[1];
		if (password.empty()) {
			user.outBuf += Server::serverMessage(ERR_NEEDMOREPARAMS, user.nick, "JOIN", "Not enough parameters");
			return;
		}
	}
	ChannelsManager& channelManager = server.getChannelsManager();
	Channel* channel = channelManager.getChannel(channelName);
	if (channel == NULL) {
		channelManager.addChannel(channelName, password);
		channel = channelManager.getChannel(channelName);
	}
	JoinResult joinResult = channelManager.canJoin(channelName, &user, password);
	switch (joinResult) {
		case JOIN_SUCCESS:
			break;
		case JOIN_BANNED:
			user.outBuf += Server::serverMessage(ERR_BANNEDFROMCHAN, user.nick, channelName, "Cannot join channel (+b)");
			return;
		case JOIN_INVITE_ONLY:
			user.outBuf += Server::serverMessage(ERR_INVITEONLYCHAN, user.nick, channelName, "Cannot join channel (+i)");
			return;
		case JOIN_BAD_PASSWORD:
			user.outBuf += Server::serverMessage(ERR_BADCHANNELKEY, user.nick, channelName, "Cannot join channel (+k)");
			return;
		case JOIN_CHANNEL_FULL:
			user.outBuf += Server::serverMessage(ERR_CHANNELISFULL, user.nick, channelName, "Cannot join channel (+l)");
			return;
	}
	channelManager.addUserToChannel(channelName, &user);
	channelManager.sendJoinNotification(channelName, &user);
}

void Executer::_executeKick(Command& command, User &user, Server &server)
{
	if (!_requireRegistered(user, "KICK")) return;

	if (command.args.size() < 2) {
		user.outBuf += Server::serverMessage(ERR_NEEDMOREPARAMS, user.nick, "KICK", "Not enough parameters");
		return;
	}

	const std::string &channelName = command.args[0];
	const std::string &targetNick = command.args[1];
	std::string reason = (command.args.size() > 2 && !command.args[2].empty()) ? command.args[2] : user.nick;

	ChannelsManager &channelManager = server.getChannelsManager();
	Channel *channel = channelManager.getChannel(channelName);
	if (!channel) {
		user.outBuf += Server::serverMessage(ERR_NOSUCHCHANNEL, user.nick, channelName, "No such channel");
		return;
	}

	if (!channelManager.isOperator(channelName, &user)) {
		user.outBuf += Server::serverMessage(ERR_CHANOPRIVSNEEDED, user.nick, channelName, "You're not channel operator");
		return;
	}

	std::map<int, User> &users = server.getUsers();
	User *targetUser = Tools::findUserByNick(targetNick, users);
	if (!targetUser) {
		user.outBuf += Server::serverMessage(ERR_NOSUCHNICK, user.nick, targetNick, "No such nick/channel");
		return;
	}

	if (std::find(channel->users.begin(), channel->users.end(), targetUser) == channel->users.end()) {
		user.outBuf += Server::serverMessage(ERR_USERNOTINCHANNEL, user.nick, targetNick + " " + channelName, "They aren't on that channel");
		return;
	}

	// Message de kick à tous les utilisateurs du canal
	std::string kickMsg = ":" + user.nick + " KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";
	channelManager.notifyChannel(channelName, kickMsg, NULL);

	// Retrait de l'utilisateur du canal après notification
	channelManager.removeUserFromChannel(channelName, targetUser);

	// Supprime le canal si vide
	if (channel->users.empty())
		channelManager.removeChannel(channelName);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Executer::_executePing(Command& command, User &user) {
	std::cout << "Executing PING command" << std::endl;
	if (command.args.size() < 1) {
		user.outBuf += Server::serverMessage(ERR_NEEDMOREPARAMS, user.nick, "PING", "Not enough parameters");
		return;
	}
	user.outBuf += "PONG :" + command.args[0] + "\r\n";
	user.lastActivityMs = Tools::nowMs();
}

void Executer::_executePong(Command& command, User &user) {
	std::cout << "Executing PONG command" << std::endl;
	if (command.args.size() < 1) {
		user.outBuf += Server::serverMessage(ERR_NEEDMOREPARAMS, user.nick, "PONG", "Not enough parameters");
		return;
	}
	user.lastActivityMs = Tools::nowMs();
	user.awaitingPong = false;
	user.pingSentMs = user.lastActivityMs;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
