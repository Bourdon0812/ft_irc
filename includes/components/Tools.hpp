#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "irc.hpp"

class Tools {
public:
	static std::string toUpper(std::string s);
	static CommandType cmdFromString(const std::string &name);
	static std::string intToString(int n);
	static User *findUserByNick(const std::string &nick, std::map<int, User> &users);
	static unsigned long nowMs();
};

#endif


