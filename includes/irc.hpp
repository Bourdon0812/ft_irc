#ifndef IRC_HPP
#define IRC_HPP

#include <algorithm>
#include <iostream>
#include <string>
#include <cstdlib>
#include <map>
#include <vector>
#include <set>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <cctype>
#include <cerrno>
#include "data/structs.hpp"
#include "data/replies.hpp"

#include "components/ChannelsManager.hpp"
#include "components/Executer.hpp"
#include "components/Parser.hpp"
#include "components/Tools.hpp"

#endif