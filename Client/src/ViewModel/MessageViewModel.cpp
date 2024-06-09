#include "ViewModel/MessageViewModel.h"
#include "ViewModel/UserViewModel.h"

#include <iostream>

namespace Piero {

MessageViewModel* MessageViewModel::s_Instance = nullptr;
MessageViewModel* MessageViewModel::GetInstance() {
    if(s_Instance == nullptr) {
        s_Instance = new MessageViewModel();
        std::cout << "Create a new MessageViewModel" << std::endl;
    }
    return s_Instance;
}

MessageViewModel::MessageViewModel() {}

MessageViewModel::~MessageViewModel() {}

void MessageViewModel::OnGetOrCreateConversationOfTwoUser(int userId1, int userId2) {
    json newJsonObject;
    newJsonObject["command"] = Client::Command::GET_OR_CREATE_CONVERSATION_OF_TWO_USER;
    newJsonObject["user_id_1"] = userId1;
    newJsonObject["user_id_2"] = userId2;
    Client::GetInstance()->SendCommand(newJsonObject.dump());
}


void MessageViewModel::OnGetConversation(int userId, int conversationId) {
    json newJsonObject;
    newJsonObject["command"] = Client::Command::GET_CONVERSATION;
    newJsonObject["user_id"] = userId;
    newJsonObject["conversation_id"] = conversationId;

    Client::GetInstance()->SendCommand(newJsonObject.dump());
}



}

