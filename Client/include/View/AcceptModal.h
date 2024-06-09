#pragma once
#include <iostream>

namespace Piero {
class AcceptModal {
public:
    static void Render();
public:
    static bool isShowing;
    static std::string message;
    static int conversationId;
};


}