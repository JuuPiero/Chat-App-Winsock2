#include "Core/Client/Client.h"
#include "PCH.h"
#include "ViewModel/UserViewModel.h"
#include "ViewModel/MessageViewModel.h"
namespace Piero {
void Client::OnLoginSuccess() {
    m_clientSocketId = responseData["client_id"];
    UserViewModel::GetInstance()->SetLogin(true);
    UserViewModel::GetInstance()->CurrentUser = User(responseData["info"]["id"], responseData["info"]["username"]); 
    UserViewModel::GetInstance()->SetPassword(std::string(UserViewModel::GetInstance()->passwordBufferInput));
    for (const auto& conversation : responseData["conversations"]) {
        MessageViewModel::GetInstance()->Conversations.push_back(Conversation(conversation["id"], conversation["name"], conversation["is_group"]));
    }
    std::cout << "Client Id : " << m_clientSocketId << std::endl; 
}

void Client::OnLoginFail() {
    UserViewModel::GetInstance()->Reset();
    MessageViewModel::GetInstance()->Reset();
}
void Client::OnLogoutSuccess() {
    // Close(); // test for attt
    UserViewModel::GetInstance()->Reset();
    MessageViewModel::GetInstance()->Reset();
}

void Client::OnReceiveMessageSuccess() {
    auto currentConversation = MessageViewModel::GetInstance()->CurrentConversation;
    auto currentUser = UserViewModel::GetInstance()->CurrentUser;
    auto conversations = MessageViewModel::GetInstance()->Conversations;
    if(currentConversation.id == responseData["conversation_id"]) {
        if(currentUser.username != responseData["sender"]) {
            auto newMessage = Message(responseData["sender"], responseData["message"]);
            newMessage.createdAt = responseData["created_at"];
            newMessage.isQuitMessage = responseData["is_quit_message"];
            MessageViewModel::GetInstance()->Messages.push_back(newMessage);
            return;
        }
        else {
            MessageViewModel::GetInstance()->Messages.push_back(Message(currentUser.username, responseData["message"]));
            return;
        }
    }

    if(!responseData["is_group"]) {
        int idCheck = responseData["conversation_id"];
        auto it = std::find_if(conversations.begin(), conversations.end(), [idCheck](const Conversation& conversation) {
            return conversation.id == idCheck;
        });
        if(it == conversations.end()) {
            MessageViewModel::GetInstance()->Conversations.push_back(Conversation(responseData["conversation_id"], responseData["sender"], false));
        }
    }
}

void Client::OnAcceptJoinConversation() {
    MessageViewModel::GetInstance()->Messages.clear();
    MessageViewModel::GetInstance()->CurrentConversation.members.clear();
    MessageViewModel::GetInstance()->CurrentConversation = Conversation(responseData["conversation_id"], responseData["name"], responseData["is_group"]);
    MessageViewModel::GetInstance()->Conversations.push_back(MessageViewModel::GetInstance()->CurrentConversation);

    for (const auto& mess : responseData["messages"]) {
        auto newMessage = Message(mess["message_id"], mess["username"], mess["content"], mess["created_at"]);
        MessageViewModel::GetInstance()->Messages.push_back(newMessage);
    }
    for (const auto& member : responseData["members"]) {
        MessageViewModel::GetInstance()->CurrentConversation.members.push_back(User(member["user_id"], member["username"]));
    }
}

void Client::OnGetConversationSuccess() {
    MessageViewModel::GetInstance()->Messages.clear();
    MessageViewModel::GetInstance()->CurrentConversation.members.clear();
    MessageViewModel::GetInstance()->CurrentConversation = Conversation(responseData["conversation_id"], responseData["name"], responseData["is_group"]);
    for (const auto& mess : responseData["messages"]) {
        MessageViewModel::GetInstance()->Messages.push_back(Message(mess["message_id"],mess["username"], mess["content"], mess["created_at"]));
    }
    for (const auto& member : responseData["members"]) {
        MessageViewModel::GetInstance()->CurrentConversation.members.push_back(User(member["user_id"], member["username"]));
    }
}
void Client::OnGetUsersOnlineSuccess() {
    for (const auto& user : responseData["users"]) {
        UserViewModel::GetInstance()->UsersOnline.push_back(User(user["id"], user["username"]));
    }
}

}
