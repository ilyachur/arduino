#pragma once

#include <map>
#include <string>

struct Event {
    std::string from;
    // empty means broadcast
    std::string to;
    std::map<std::string, std::string> args;
};
