#include "Core/Server/Server.h"
#include "spdlog/spdlog.h"
#include "Core/Model/Conversation.h"
#include "PCH.h"
namespace Piero {

void Server::OnUserLogin(uint32_t clientSocketId, std::string username, std::string password) {
    auto clientSocket = m_ClientSockets[clientSocketId];
   
    spdlog::info("Request login from user: {}", username);
    json newJsonObject;
    auto user = User::GetUserByUserName(username);
   
    if(!user) {
        spdlog::warn("User not exist");
        newJsonObject["status"] = StatusCode::LOGIN_FAILED_USER_NOT_EXIST;
        newJsonObject["message"] = "User not exist";
        iResult = send(clientSocket, newJsonObject.dump().c_str(), newJsonObject.dump().size(), 0);
        return;
    }

    if(user->GetPassword() != password) {
        newJsonObject["status"] = StatusCode::LOGOUT_FAILED_PASSWORD;
        newJsonObject["message"] = "Password is incorrect";
        spdlog::warn("Password is incorrect");
    }
    else {
        m_UserOnlines[user->GetUsername()] = clientSocketId;
        user->SetOnline();

        newJsonObject["status"] = StatusCode::LOGIN_SUCCESS;
        newJsonObject["message"] = "User: " + user->GetUsername() + " login successully ";

        newJsonObject["client_id"] = clientSocketId;

        newJsonObject["info"] = {
            { "id", user->GetId() }, 
            { "username", user->GetUsername() }, 
            { "status", user->GetStatus() },
        };
        auto conversations = Conversation::GetAllConversationsByUserId(user->GetId());
        for (size_t i = 0; i < conversations.size(); i++) {
            json conversationObject;
            conversationObject["id"] = conversations[i]->GetId();
            std::string conversationName = conversations[i]->GetName();
            if(conversationName.empty()) { // this conversation is c of 2 users
                conversationName = Conversation::GetReceiverUsername(user->GetId(), conversations[i]->GetId());
            }
            conversationObject["name"] = conversationName;
            conversationObject["is_group"] = conversations[i]->IsGroup();
            newJsonObject["conversations"][i] = conversationObject;
        }
        // m_UserOnlines.emplace(username, clientSocketId);
        spdlog::info("User: {} login successully ", user->GetUsername());

    }
    iResult = send(clientSocket, newJsonObject.dump().c_str(), newJsonObject.dump().size(), 0);
    if (iResult == SOCKET_ERROR) {
        spdlog::error("send failed: ", WSAGetLastError());
    }
}

void Server::OnUserLogout(SOCKET clientSocket, std::string& username, std::string& password) {
    // SOCKET clientSocket = m_ClientSockets[clientSocketId];
    
    auto user = User::GetUserByUserName(username);
    spdlog::trace("Request logout from user: {}", username);
    user->SetOffline();

    json newJsonObject;
    newJsonObject["status"] = StatusCode::LOGOUT_SUCCESS;
    newJsonObject["message"] = "User: " + user->GetUsername() + " logout successully ";

    if(m_UserOnlines[user->GetUsername()]) {
        m_UserOnlines.erase(user->GetUsername());
    }

    iResult = send(clientSocket, newJsonObject.dump().c_str(),  newJsonObject.dump().size(), 0);
    if (iResult == SOCKET_ERROR) {
        spdlog::error("send failed: ", WSAGetLastError());
    }
    // if(!m_UserOnlines[username].empty()) {
    //     m_UserOnlines.erase(username);
    // }

    spdlog::info("User logout successully: {} ", username);
}


void Server::OnUserSendMessage(uint32_t clientSocketId, int senderId, int conversationId, std::string& message, bool isQuitMessage) {
    // auto username = User::GetUsernameById(senderId);
    auto clientSocket = m_ClientSockets[clientSocketId];
    auto conversation = Conversation::GetConversationById(conversationId);
    spdlog::info("{} send message: {} to conversation: {}", User::GetUsernameById(senderId), message, conversationId);
    if(!isQuitMessage) {
        std::string query = "INSERT INTO messages (conversation_id, user_id, content) VALUES (" + std::to_string(conversationId) + ", " + std::to_string(senderId) + ", '" + message + "')";
        Connection::GetInstance()->Query(query.c_str());
    }
    json newJsonObject;
    newJsonObject["status"] = StatusCode::RECEIVE_MESSAGE_SUCCESS;
    newJsonObject["message"] = std::string(message);
    newJsonObject["sender"] = User::GetUsernameById(senderId);
    newJsonObject["created_at"] = getCurrentTime();
    newJsonObject["conversation_id"] = conversationId;
    newJsonObject["is_group"] = conversation->IsGroup();
    newJsonObject["is_quit_message"] = isQuitMessage;


    if(!conversation->IsGroup()) {
        newJsonObject["receiver_id"] = Conversation::GetReceiverId(senderId, conversationId);
    }
    
    auto members = Conversation::GetMembersExcept(senderId, conversationId);

    for (auto& memberId : members) {
        auto username = User::GetUsernameById(memberId);
        if(m_UserOnlines[username]) {
            if(m_ClientSockets[m_UserOnlines[username]]) {
                iResult = send(m_ClientSockets[m_UserOnlines[username]], newJsonObject.dump().c_str(), newJsonObject.dump().size(), 0);
                if (iResult == SOCKET_ERROR) {
                    std::cout << "send failed: " << WSAGetLastError() << std::endl;
                }
            }
        }
    }
}

void Server::OnUserGetUsersOnline(uint32_t clientSocketId, int userId) {
    SOCKET clientSocket = m_ClientSockets[clientSocketId];
    spdlog::info("{} request get all users onilne ", User::GetUsernameById(userId));
    json newJsonObject;
    newJsonObject["status"] = StatusCode::GET_USERS_ONLINE_SUCCESS;
    newJsonObject["message"] = std::string("Get all users online successfully");
    auto usersOnline =  User::GetAllUserOnline(userId);
    for (size_t i = 0; i < usersOnline.size(); i++) {
        json userObject;
        userObject["client_id"] = m_UserOnlines[usersOnline[i]->GetUsername()];
        userObject["id"] = usersOnline[i]->GetId();
        userObject["username"] = usersOnline[i]->GetUsername();
        newJsonObject["users"][i] = userObject;
    }

    iResult = send(clientSocket, newJsonObject.dump().c_str(), newJsonObject.dump().size(), 0);
    if (iResult == SOCKET_ERROR) {
        std::cout << "send failed: " << WSAGetLastError() << std::endl;
    }
}

void Server::OnUserGetConversation(SOCKET& clientSocket, int userId, int conversationId) {
    // SOCKET clientSocket = m_ClientSockets[clientSocketId];
    spdlog::info("Request get all messages of conservation with id: {} from user: {}", conversationId, User::GetUsernameById(userId));
    auto conversation = Conversation::GetConversationById(conversationId);
    json newJsonObject;
    if(!conversation->IsGroup()) {
        std::string name = Conversation::GetReceiverUsername(userId, conversationId);
        conversation->SetName(name);
        newJsonObject["receiver_id"] = Conversation::GetReceiverId(userId, conversationId);
    }
    else {
        newJsonObject["receiver_id"] = 0;
    }
    
    newJsonObject["status"] = StatusCode::GET_CONVERSATION_SUCCSESS;
    newJsonObject["message"] = "Get Conversation successfully";

    newJsonObject["conversation_id"] = conversationId;
    newJsonObject["name"] = conversation->GetName();
    newJsonObject["is_group"] = conversation->IsGroup();

    auto messages = Conversation::GetMessages(conversationId);
    
    
    for (size_t i = 0; i < messages.size(); i++) {
        json messageObject;
        messageObject["message_id"] = messages[i]->GetId();
        messageObject["username"] = User::GetUsernameById(messages[i]->GetUserId());
        messageObject["content"] = messages[i]->GetContent();
        messageObject["created_at"] = messages[i]->CreatedAt();
        newJsonObject["messages"][i] = messageObject;
    }
    newJsonObject["messages_count"] = messages.size();


    auto members = Conversation::GetMembersExcept(userId, conversationId);
    for (size_t i = 0; i < members.size(); i++) {
        json memberObject;
        memberObject["user_id"] = members[i];
        memberObject["username"] = User::GetUsernameById(members[i]);
        newJsonObject["members"][i] = memberObject;
    }
    iResult = send(clientSocket, newJsonObject.dump().c_str(), newJsonObject.dump().size(), 0);
    if (iResult == SOCKET_ERROR) {
        std::cout << "send failed: " << WSAGetLastError() << std::endl;
    }
}


void Server::OnUserGetConversationOfTwoUsers(uint32_t clientSocketId, int userId1, int userId2) {
    
    spdlog::info("{} get conversation with {} ", User::GetUsernameById(userId1), User::GetUsernameById(userId2));
    auto conversation = Conversation::GetConversationOfTwoUsers(userId1, userId2);
    int conversationId = 0;
    json newJsonObject;

    if (!conversation) {
        std::cout << "The Conversation is not exsist." << std::endl;
        conversationId = Conversation::CreateConversationOfTwoUsers(userId1, userId2);
        newJsonObject["exist"] = false;
    } else {
        conversationId = conversation->GetId();
        std::cout << "The Conversation is exsisted." << std::endl;
        newJsonObject["exist"] = true;
    }
    
    newJsonObject["status"] = StatusCode::GET_OR_CREATE_CONVERSATION_OF_TWO_USER_SUCCSESS;
    newJsonObject["message"] = User::GetUsernameById(userId1) + " get conversation with " + User::GetUsernameById(userId2) + " successfully";
    newJsonObject["conversation_id"] = conversationId;
    newJsonObject["conversation_name"] = User::GetUsernameById(userId2);
    newJsonObject["user_id_1"] = userId1;
    newJsonObject["user_id_2"] = userId2;

    auto username1 = User::GetUsernameById(userId1);
    auto username2 = User::GetUsernameById(userId2);

    if(m_UserOnlines[username1]) {
        iResult = send(m_ClientSockets[m_UserOnlines[username1]], newJsonObject.dump().c_str(), newJsonObject.dump().size(), 0);
        if (iResult == SOCKET_ERROR) {
            std::cout << "send failed: " << WSAGetLastError() << std::endl;
        }
    }

    if(m_UserOnlines[username2]) {
        iResult = send(m_ClientSockets[m_UserOnlines[username2]], newJsonObject.dump().c_str(), newJsonObject.dump().size(), 0);
        if (iResult == SOCKET_ERROR) {
            std::cout << "send failed: " << WSAGetLastError() << std::endl;
        }
    }
}

void Server::OnUserCreateConversation(SOCKET& clientSocket, int userId, std::string convsersationName) {
    spdlog::info("{} create conversation with name: {}", User::GetUsernameById(userId), convsersationName);

    json responseJson;

    int conversationId = Conversation::CreateConversation(userId, convsersationName, true);

    responseJson["status"] =  StatusCode::CREATE_CONVERSATION_SUCCSESS;
    responseJson["message"] =  User::GetUsernameById(userId) + " created group chat name:  " + convsersationName + " successfully";


    responseJson["conversation_id"] = conversationId;
    responseJson["conversation_name"] = convsersationName;
    responseJson["is_group"] = true;

    iResult = send(clientSocket, responseJson.dump().c_str(), responseJson.dump().size(), 0);
    if (iResult == SOCKET_ERROR) {
        std::cout << "send failed: " << WSAGetLastError() << std::endl;
    }

}


void Server::OnUserInviteUserToConversation(SOCKET& clientSocket, int senderId, int userId, int conversationId) {
    auto conversation = Conversation::GetConversationById(conversationId);
    spdlog::info("User: {} invite user: {} join in conversation : {}", User::GetUsernameById(senderId), User::GetUsernameById(userId), conversation->GetName());

    json responseJson;

    responseJson["status"] =  StatusCode::SEND_INVITATION_TO_JOIN_CONVERSATION_SUCCSESS;
    responseJson["message"] = User::GetUsernameById(senderId) + " invites you to join the conversation :" + conversation->GetName();
    responseJson["sender_id"] =  senderId;
    responseJson["user_id"] =  userId;
    responseJson["conversation_id"] =  conversationId;
    responseJson["conversation_name"] =  conversation->GetName();
    responseJson["is_group"] =  true;

    auto username = User::GetUsernameById(userId);

    if(m_UserOnlines[username]) {
        iResult = send(m_ClientSockets[m_UserOnlines[username]], responseJson.dump().c_str(), responseJson.dump().size(), 0);
        if (iResult == SOCKET_ERROR) {
            std::cout << "send failed: " << WSAGetLastError() << std::endl;
        }
    }

}


void Server::OnUserAcceptInvite(SOCKET& clientSocket, int userId, int conversationId) {
    auto conversation = Conversation::GetConversationById(conversationId);
    std::string query = "INSERT INTO participants (user_id, conversation_id) VALUES (" + std::to_string(userId) +", " + std::to_string(conversationId) + ")";
    Connection::GetInstance()->Query(query.c_str());


    spdlog::info("User: {} accepted to join in conversation : {}", User::GetUsernameById(userId), User::GetUsernameById(userId), conversation->GetName());
   
    json newJsonObject;

    newJsonObject["user_id"] = userId;
    newJsonObject["status"] = StatusCode::ACCEPT_JOIN_IN_CONVERSATION_SUCCSESS;
    newJsonObject["message"] = "accepted to join in conversation successfully";

    newJsonObject["conversation_id"] = conversationId;
    newJsonObject["name"] = conversation->GetName();
    newJsonObject["is_group"] = conversation->IsGroup();

    auto messages = Conversation::GetMessages(conversationId);
    
    for (size_t i = 0; i < messages.size(); i++) {
        json messageObject;
        messageObject["message_id"] = messages[i]->GetId();
        messageObject["username"] = User::GetUsernameById(messages[i]->GetUserId());
        messageObject["content"] = messages[i]->GetContent();
        messageObject["created_at"] = messages[i]->CreatedAt();
        newJsonObject["messages"][i] = messageObject;
    }
    newJsonObject["messages_count"] = messages.size();


    auto members = Conversation::GetMembersExcept(userId, conversationId);
    for (size_t i = 0; i < members.size(); i++) {
        json memberObject;
        memberObject["user_id"] = members[i];
        memberObject["username"] = User::GetUsernameById(members[i]);
        newJsonObject["members"][i] = memberObject;
    }
    iResult = send(clientSocket, newJsonObject.dump().c_str(), newJsonObject.dump().size(), 0);
    if (iResult == SOCKET_ERROR) {
        std::cout << "send failed: " << WSAGetLastError() << std::endl;
    }
}

}