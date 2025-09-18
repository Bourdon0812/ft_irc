/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 13:54:09 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/09/16 11:20:39 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/*
// A créer (version chatgpt temporaire pour avancer sur le reste)
Command	Parser::parse(std::string inBuf)
{
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
*/

Command	Parser::parse(const std::string& input)
{
	Command		cmd;
	std::string	line;
	size_t		i;
	size_t		next;
	std::string	cmdStr;
	size_t		sp;

	cmd.originalInput = input;
	line = input;
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);
	i = 0;
	if (!line.empty() && line[0] == ':')
	{
		size_t	sp = line.find(' ');
		if (sp != std::string::npos)
		{
			cmd.prefix = line.substr(1, sp - 1);
			i = sp + 1;
		}
		else
		{
			cmd.type = UNKNOWN;
			return (cmd);
		}
	}
	while (i != line.size() && line[i] == ' ')
		++i;
	next = line.find(' ', i);
	if (next == std::string::npos)
		cmdStr = line.substr(i);
	else
		cmdStr = line.substr(i, next - i);
	for (size_t k = 0; k != cmdStr.size(); ++k)
		cmdStr[k] = std::toupper(cmdStr[k]);
	cmd.type = Tools::cmdFromString(cmdStr);
	i += cmdStr.size();
	while (i != line.size() && line[i] == ' ')
		++i;
	while (i != line.size())
	{
		if (line[i] == ':')
		{
			cmd.args.push_back(line.substr(i + 1));
			break ;
		}
		sp = line.find(' ', i);
		if (sp == std::string::npos)
		{
			cmd.args.push_back(line.substr(i));
			break ;
		}
		else
		{
			cmd.args.push_back(line.substr(i, sp - i));
			i = sp + 1;
			while (i != line.size() && line[i] == ' ')
				++i;
		}
	}
	return (cmd);
}
