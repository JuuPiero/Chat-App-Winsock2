#pragma once
#include <unordered_set>
#include <vector>

namespace Piero {

class CreateGroupChatForm {
public:
    static void Render();

public:
    static bool isShowing;
    static char groupNameInputBuffer[100];
    // static std::vector<bool> toggleStates;

};




}