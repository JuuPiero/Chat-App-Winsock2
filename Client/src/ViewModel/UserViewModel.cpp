#include "ViewModel/UserViewModel.h"
#include "ViewModel/MessageViewModel.h"
#include "Core/Client/Client.h"
#include "View/AcceptModal.h"
namespace Piero {
UserViewModel* UserViewModel::s_Instance = nullptr;

UserViewModel* UserViewModel::GetInstance() {
    if(s_Instance == nullptr) s_Instance = new UserViewModel();
    return s_Instance;
}
void UserViewModel::OnUserLogin(const std::string& username, const std::string& password) {
    m_Password = password;
    json requestJson;
    requestJson["command"] = Client::Command::LOGIN;
    requestJson["data"] = { 
        { "username", username }, 
        { "password", password } 
    };
    Client::GetInstance()->SendCommand(requestJson.dump());
}
void UserViewModel::OnUserLogout() {

    if(MessageViewModel::GetInstance()->CurrentConversation.id) {
        OnSendMessage(MessageViewModel::GetInstance()->CurrentConversation.id, "OFFLINE", true);
    }

    json requestJson;
    requestJson["command"] = Client::Command::LOGOUT;
    requestJson["data"] = { 
        { "username", CurrentUser.username }, 
        { "password", m_Password } 
    };
    Client::GetInstance()->SendCommand(requestJson.dump());
    m_IsLoggedIn = false;
}

void UserViewModel::OnSendMessage(int conversationId, std::string message, bool isQuitMessage) {
    json newJsonObject;
    newJsonObject["command"] = Client::Command::SEND_MESSAGE;
    newJsonObject["conversation_id"] = conversationId;
    newJsonObject["sender_id"] = CurrentUser.id;
    newJsonObject["message"] = message;
    newJsonObject["is_quit_message"] = isQuitMessage;
    Client::GetInstance()->SendCommand(newJsonObject.dump());
}

void UserViewModel::OnGetAllUsersOnline() {
    json newJsonObject;
    newJsonObject["command"] = Client::Command::GET_USERS_ONLINE;
    newJsonObject["user_id"] = CurrentUser.id;
    Client::GetInstance()->SendCommand(newJsonObject.dump());
}


void UserViewModel::OnCreateConversation(const std::string& name) {
    json requestJson;
    requestJson["command"] = Client::Command::CREATE_CONVERSATION;
    requestJson["user_id"] = CurrentUser.id;
    requestJson["conversation_name"] = name;
    requestJson["is_group"] = true;
    Client::GetInstance()->SendCommand(requestJson.dump());
}


void UserViewModel::OnInviteUser(int conversationId, const int& userId) {
    json requestJson;
    requestJson["command"] = Client::Command::INVITE_USER_TO_CONVERSATION;
    requestJson["sender_id"] = CurrentUser.id;
    requestJson["conversation_id"] = conversationId;
    requestJson["user_id"] = userId;
    Client::GetInstance()->SendCommand(requestJson.dump());
}

void UserViewModel::OnAcceptInvite(int conversationId) {
    json requestJson;
    requestJson["command"] = Client::Command::ACCEPT_INVITE;
    requestJson["conversation_id"] = conversationId;
    requestJson["user_id"] = CurrentUser.id;
    Client::GetInstance()->SendCommand(requestJson.dump());
}

void UserViewModel::OnLeftGroupChat() {
    json requestJson;
    requestJson["command"] = Client::Command::LEFT_GROUP_CHAT;
    requestJson["conversation_id"] = MessageViewModel::GetInstance()->CurrentConversation.id;
    requestJson["user_id"] = CurrentUser.id;
    Client::GetInstance()->SendCommand(requestJson.dump());
}

void UserViewModel::OnSignup(const std::string& username, const std::string& password) {
    json requestJson;
    requestJson["command"] = Client::Command::SIGNUP;
    requestJson["data"] = { 
        { "username", username }, 
        { "password", password } 
    };
    Client::GetInstance()->SendCommand(requestJson.dump());
}

}