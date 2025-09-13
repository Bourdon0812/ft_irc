#ifndef CHANNELS_MANAGER_HPP
#define CHANNELS_MANAGER_HPP

#include "irc.hpp"

class ChannelsManager {
private:
	std::map<std::string, Channel> _channels;
public:
	ChannelsManager();
	~ChannelsManager();

	// Gestion des channels
	Channel* getChannel(const std::string &name);
	void addChannel(const std::string &name, const std::string password);
	void removeChannel(const std::string &name);
	void addUserToChannel(const std::string &channelName, User *user);
	void removeUserFromChannel(const std::string &channelName, User *user);

	// Gestion des permissions
	bool isOperator(const std::string &channelName, User *user);
	bool isVoiced(const std::string &channelName, User *user);
	void addOperator(const std::string &channelName, User *user);
	void removeOperator(const std::string &channelName, User *user);
	void addVoiced(const std::string &channelName, User *user);
	void removeVoiced(const std::string &channelName, User *user);
	
	// Gestion des modes
	bool hasMode(const std::string &channelName, char mode);
	void setMode(const std::string &channelName, char mode, bool on);
	void setMode(const std::string &channelName, char mode, bool on, const std::string &param);
	
	// Gestion des invitations
	void addInvite(const std::string &channelName, const std::string &nickname);
	void removeInvite(const std::string &channelName, const std::string &nickname);
	bool isInvited(const std::string &channelName, const std::string &nickname);
	
	// Gestion des bannissements
	void addBan(const std::string &channelName, const std::string &mask);
	void removeBan(const std::string &channelName, const std::string &mask);
	bool isBanned(const std::string &channelName, const std::string &mask);
	
	// Vérifications pour JOIN
	bool canJoin(const std::string &channelName, User *user, const std::string &password);
    
};

#endif