#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include "irc.hpp"

// struct used for the a client (user)
struct User {
    bool passOK;
    bool registered;
    bool quit;

    std::string nick;
    std::string username;
    std::string realname;
    std::string hostname;

    std::string inBuf;
    std::string outBuf;

    std::set<std::string> channels;

    User() : passOK(false), registered(false), quit(false), channels() {}
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