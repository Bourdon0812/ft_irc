#include "components/Parser.hpp"
#include "components/Tools.hpp"
#include <sstream>

// A créer (version chatgpt temporaire pour avancer sur le reste)
Command Parser::parse(std::string inBuf) {
	std::cout << "Parsing input buffer: " << inBuf << std::endl;
	Command cmd;
	cmd.originalInput = inBuf;

	// déterminer la commande (1er mot)
	std::istringstream iss(inBuf);
	std::string word;
	if (!(iss >> word)) {
		cmd.type = UNKNOWN;
		return cmd;
	}
	cmd.type = Tools::cmdFromString(word);

	// args: tous les mots suivants, avec support d'un trailing param commençant par ':'
	while (iss >> word) {
		if (!word.empty() && word[0] == ':') {
			std::string rest;
			std::getline(iss, rest);
			if (!rest.empty() && rest[0] == ' ') rest.erase(0, 1);
			cmd.args.push_back(word.substr(1) + (rest.empty() ? std::string("") : std::string(" ") + rest));
			break;
		}
		cmd.args.push_back(word);
	}
	return cmd;
}