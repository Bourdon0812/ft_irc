#ifndef PARSER_HPP
#define PARSER_HPP

#include "irc.hpp"

// class used to parse the input buffer
class Parser {

// je l'ai mis en place histoire de commencer
public:
	static Command parse(std::string inBuf);
};

#endif