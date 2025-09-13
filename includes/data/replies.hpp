#ifndef REPLIES_HPP
#define REPLIES_HPP

// 001-004: welcome burst
#define RPL_WELCOME                 1  // ":Welcome to the Internet Relay Network <nick>!<user>@<host>"
#define RPL_YOURHOST                2  // ":Your host is <server>, running version <ver>"
#define RPL_CREATED                 3  // ":This server was created <date>"
#define RPL_MYINFO                  4  // "<server> <ver> <user_modes> <chan_modes> [<chan_modes_with_param>]"

// Channel info
#define RPL_JOIN                    0    // ":<nick>!<user>@<host> JOIN :<channel>"
#define RPL_TOPIC                   332  // "<channel> :<topic>"
#define RPL_NOTOPIC                 331  // "<channel> :No topic is set"
#define RPL_NAMREPLY                353  // "<channel> :<nick> <nick> ..."
#define RPL_ENDOFNAMES              366  // "<channel> :End of /NAMES list"

// MOTD
#define RPL_MOTDSTART               375  // ":- <server> Message of the day - "
#define RPL_MOTD                    372  // ":- <text>"
#define RPL_ENDOFMOTD               376  // ":End of /MOTD command."

// Operator 
#define RPL_YOUREOPER               381

// Errors
#define ERR_NOSUCHNICK              401  // "<nick> :No such nick/channel"
#define ERR_NOSUCHCHANNEL           403  // "<channel> :No such channel"
#define ERR_CANNOTSENDTOCHAN        404  // "<channel> :Cannot send to channel"
#define ERR_UNKNOWNCOMMAND          421  // "<command> :Unknown command"
#define ERR_NONICKNAMEGIVEN         431  // ":No nickname given"
#define ERR_ERRONEUSNICKNAME        432  // "<nick> :Erroneous nickname"
#define ERR_NICKNAMEINUSE           433  // "<nick> :Nickname is already in use"
#define ERR_USERNOTINCHANNEL        441  // "<nick> <channel> :They aren't on that channel"
#define ERR_NOTONCHANNEL            442  // "<channel> :You're not on that channel"
#define ERR_USERONCHANNEL           443  // "<nick> <channel> :is already on channel"
#define ERR_NOTREGISTERED           451  // ":You have not registered"
#define ERR_NEEDMOREPARAMS          461  // "<command> :Not enough parameters"
#define ERR_ALREADYREGISTRED        462  // ":You may not reregister"
#define ERR_PASSWDMISMATCH          464  // ":Password incorrect"
#define ERR_CHANNELISFULL           471  // "<channel> :Cannot join channel (+l)"
#define ERR_INVITEONLYCHAN          473  // "<channel> :Cannot join channel (+i)"
#define ERR_BADCHANNELKEY           475  // "<channel> :Cannot join channel (+k)"
#define ERR_BANNEDFROMCHAN          474  // "<channel> :Cannot join channel (+b)"

#define PING_TIMEOUTMS             60000// ":No response from client"
#endif


