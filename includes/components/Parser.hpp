#ifndef PARSER_HPP
#define PARSER_HPP

#include "irc.hpp"

// class used to parse the input buffer
class Parser {
public:
	static void parse(std::string inBuf);
};

#endif