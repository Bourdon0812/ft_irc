#include "components/Tools.hpp"

std::string Tools::toUpper(std::string s) {
	for (size_t i = 0; i < s.size(); ++i) {
		s[i] = std::toupper(static_cast<unsigned char>(s[i]));
	}
	return s;
}

CommandType Tools::cmdFromString(const std::string &name) {
	std::string n = Tools::toUpper(name);
	if (n == "PASS")    return PASS;
	if (n == "NICK")    return NICK;
	if (n == "USER")    return USER;
	if (n == "JOIN")    return JOIN;
	if (n == "PRIVMSG") return PRIVMSG;
	if (n == "KICK")    return KICK;
	if (n == "INVITE")  return INVITE;
	if (n == "TOPIC")   return TOPIC;
	if (n == "MODE")    return MODE;
	if (n == "PING")    return PING;
	if (n == "PONG")    return PONG;
	return UNKNOWN;
}

std::string Tools::intToString(int n) {
	std::stringstream ss;
	ss << n;
	return ss.str();
}

unsigned long Tools::nowMs() {
	timeval tv; gettimeofday(&tv, 0);
	return (unsigned long)(tv.tv_sec) * 1000ul + (unsigned long)(tv.tv_usec) / 1000ul;
  }