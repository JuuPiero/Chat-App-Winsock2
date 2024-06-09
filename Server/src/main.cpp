#include "PCH.h"
int main(int argc, char *argv[]) {
    auto app = Piero::Application::GetInstance();

    auto test = Piero::Conversation::GetConversationOfTwoUsers(1, 2);
    app->Run([]() {
        
    });
    app->ShutDown();
    return 0;
}