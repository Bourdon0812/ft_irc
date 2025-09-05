#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <string>
#include <set>

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

    User() : passOK(false), registered(false), quit(false) {}
};

#endif