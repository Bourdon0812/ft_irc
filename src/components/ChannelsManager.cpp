#include "components/ChannelsManager.hpp"
#include "components/Server.hpp"

ChannelsManager::ChannelsManager() : _channels() {}

ChannelsManager::~ChannelsManager() {}

Channel* ChannelsManager::getChannel(const std::string &name) {
	std::map<std::string, Channel>::iterator it = this->_channels.find(name);
	if (it == this->_channels.end()) return NULL;
	return &it->second;
}

void ChannelsManager::addChannel(const std::string &name, const std::string password) {
    Channel newChannel;
    newChannel.name = name;
    if (!password.empty()) {
        newChannel.password = password;
        newChannel.modes += 'k';
    }
    this->_channels[name] = newChannel;
}

void ChannelsManager::removeChannel(const std::string &name) {
	this->_channels.erase(name);
}

void ChannelsManager::addUserToChannel(const std::string &channelName, User *user) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it != this->_channels.end()) {
        it->second.users.push_back(user);
        if (it->second.users.size() == 1) {
            addOperator(channelName, user);
        }
    }
}

void ChannelsManager::removeUserFromChannel(const std::string &channelName, User *user) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it != this->_channels.end()) {
        it->second.users.erase(std::remove(it->second.users.begin(), it->second.users.end(), user), it->second.users.end());
        it->second.operators.erase(std::remove(it->second.operators.begin(), it->second.operators.end(), user), it->second.operators.end());
        it->second.voiced.erase(std::remove(it->second.voiced.begin(), it->second.voiced.end(), user), it->second.voiced.end());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ChannelsManager::isOperator(const std::string &channelName, User *user) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it == this->_channels.end()) return false;
    return std::find(it->second.operators.begin(), it->second.operators.end(), user) != it->second.operators.end();
}

bool ChannelsManager::isVoiced(const std::string &channelName, User *user) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it == this->_channels.end()) return false;
    return std::find(it->second.voiced.begin(), it->second.voiced.end(), user) != it->second.voiced.end();
}

void ChannelsManager::addOperator(const std::string &channelName, User *user) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it != this->_channels.end() && !isOperator(channelName, user)) {
        it->second.operators.push_back(user);
    }
}

void ChannelsManager::removeOperator(const std::string &channelName, User *user) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it != this->_channels.end()) {
        it->second.operators.erase(std::remove(it->second.operators.begin(), it->second.operators.end(), user), it->second.operators.end());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ChannelsManager::addVoiced(const std::string &channelName, User *user) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it != this->_channels.end() && !isVoiced(channelName, user)) {
        it->second.voiced.push_back(user);
    }
}

void ChannelsManager::removeVoiced(const std::string &channelName, User *user) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it != this->_channels.end()) {
        it->second.voiced.erase(std::remove(it->second.voiced.begin(), it->second.voiced.end(), user), it->second.voiced.end());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ChannelsManager::hasMode(const std::string &channelName, char mode) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it == this->_channels.end()) return false;
    return it->second.modes.find(mode) != std::string::npos;
}

void ChannelsManager::setMode(const std::string &channelName, char mode, bool on) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it == this->_channels.end()) return;
    
    std::string &modes = it->second.modes;
    size_t pos = modes.find(mode);
    
    if (on && pos == std::string::npos) {
        modes += mode;
    } else if (!on && pos != std::string::npos) {
        modes.erase(pos, 1);
    }
}

void ChannelsManager::setMode(const std::string &channelName, char mode, bool on, const std::string &param) {
    setMode(channelName, mode, on);
    
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it == this->_channels.end()) return;
    
    if (mode == 'k' && on) {
        it->second.password = param;
    } else if (mode == 'l' && on) {
        it->second.userLimit = atoi(param.c_str());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ChannelsManager::addInvite(const std::string &channelName, const std::string &nickname) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it != this->_channels.end()) {
        it->second.invited.insert(nickname);
    }
}

void ChannelsManager::removeInvite(const std::string &channelName, const std::string &nickname) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it != this->_channels.end()) {
        it->second.invited.erase(nickname);
    }
}

bool ChannelsManager::isInvited(const std::string &channelName, const std::string &nickname) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it == this->_channels.end()) return false;
    return it->second.invited.find(nickname) != it->second.invited.end();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ChannelsManager::addBan(const std::string &channelName, const std::string &mask) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it != this->_channels.end()) {
        it->second.banned.insert(mask);
    }
}

void ChannelsManager::removeBan(const std::string &channelName, const std::string &mask) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it != this->_channels.end()) {
        it->second.banned.erase(mask);
    }
}

bool ChannelsManager::isBanned(const std::string &channelName, const std::string &mask) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it == this->_channels.end()) return false;
    return it->second.banned.find(mask) != it->second.banned.end();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

JoinResult ChannelsManager::canJoin(const std::string &channelName, User *user, const std::string &password) {
    Channel* channel = getChannel(channelName);
    if (!channel) return JOIN_SUCCESS;
    
    if (isBanned(channelName, user->nick)) return JOIN_BANNED;
    
    if (hasMode(channelName, 'i') && !isInvited(channelName, user->nick)) {
        return JOIN_INVITE_ONLY;
    }
    
    if (hasMode(channelName, 'k') && channel->password != password) {
        return JOIN_BAD_PASSWORD;
    }
    
    if (hasMode(channelName, 'l') && static_cast<int>(channel->users.size()) >= channel->userLimit) {
        return JOIN_CHANNEL_FULL;
    }
    
    return JOIN_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ChannelsManager::notifyChannel(const std::string &channelName, const std::string &message, User *excludeUser) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it == this->_channels.end()) return;
    
    for (std::vector<User*>::iterator userIt = it->second.users.begin(); userIt != it->second.users.end(); ++userIt) {
        if (*userIt != excludeUser) {
            (*userIt)->outBuf += message;
        }
    }
}

void ChannelsManager::sendJoinNotification(const std::string &channelName, User *user) {
    std::string joinMessage = ":" + user->nick + "!" + user->username + "@" + user->hostname + " JOIN :" + channelName + "\r\n";
    notifyChannel(channelName, joinMessage, user);
}

void ChannelsManager::sendTopicInfo(const std::string &channelName, User *user) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it == this->_channels.end()) return;
    
    if (it->second.topic.empty()) {
        user->outBuf += Server::serverMessage(RPL_NOTOPIC, user->nick, channelName, "No topic is set");
    } else {
        user->outBuf += Server::serverMessage(RPL_TOPIC, user->nick, channelName, it->second.topic);
    }
}

void ChannelsManager::sendNamesList(const std::string &channelName, User *user) {
    std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);
    if (it == this->_channels.end()) return;
    
    std::string namesList = "";
    for (std::vector<User*>::iterator userIt = it->second.users.begin(); userIt != it->second.users.end(); ++userIt) {
        if (!namesList.empty()) namesList += " ";
        
        // Ajouter @ pour les opérateurs
        if (isOperator(channelName, *userIt)) {
            namesList += "@";
        }
        namesList += (*userIt)->nick;
    }
    
    user->outBuf += Server::serverMessage(RPL_NAMREPLY, user->nick, channelName, namesList);
    user->outBuf += Server::serverMessage(RPL_ENDOFNAMES, user->nick, channelName, "End of /NAMES list");
}