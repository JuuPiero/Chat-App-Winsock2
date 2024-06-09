#pragma once
#include "Model/User.h"
#include <vector>
namespace Piero {

struct Conversation {
    int id;
    std::string name;
    bool isGroup;
    std::vector<User> members;
    Conversation(int id, std::string name, bool isGroup): id(id), name(name), isGroup(isGroup) {}
    Conversation(): id(0), name(""), isGroup(false) {}
};

}