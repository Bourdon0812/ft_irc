#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include "irc.hpp"

// struct used for the a client (user)
struct User {
    bool passOK;
    bool nickOk;
	bool userOk;
	bool welcome;
    bool quit;
	bool awaitingPong;

	unsigned long lastActivityMs;
	unsigned long pingSentMs;
    std::string nick;
    std::string username;
    std::string realname;
    std::string hostname;

    std::string inBuf;
    std::string outBuf;
    User() : passOK(false), nickOk(false), userOk(false), welcome(false), quit(false), awaitingPong(false), lastActivityMs(0), pingSentMs(0) {}
};

// struct used for a channel
struct Channel {
    std::string name;
    std::string topic;
    std::string password;
    std::string modes;
    std::vector<User*> users;
    std::vector<User*> operators;
    std::vector<User*> voiced;
    std::set<std::string> invited;
    std::set<std::string> banned;
    int userLimit;
    Channel() : name(), topic(), password(), modes(), users(), operators(), voiced(), invited(), banned(), userLimit(0) {}
};

// enum used to identify the type of command
enum CommandType {
    PASS, NICK, USER,
    JOIN, PRIVMSG,
    KICK, INVITE, TOPIC, MODE,
    PING, PONG,
    UNKNOWN
};

// struct used to store the command and its arguments
struct Command {
	std::string originalInput;
	CommandType type;
	std::vector<std::string> args;
	Command() : type(UNKNOWN), args() {}
};

#endif