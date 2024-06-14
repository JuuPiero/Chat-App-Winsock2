#pragma once
#include "PCH.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Piero {
class Client {
public:
    enum Command {
        LOGIN = 0,
        LOGOUT = 1,
        SEND_MESSAGE = 2,
        SIGNUP = 3,
        GET_USERS_ONLINE = 4,
        GET_OR_CREATE_CONVERSATION_OF_TWO_USER = 5,
        INVITE_USER_TO_CONVERSATION = 6,
        GET_CONVERSATION = 20,
        CREATE_CONVERSATION = 21,
        ACCEPT_INVITE = 25,
        LEFT_GROUP_CHAT = 30        
    };
public:
    ~Client();
    void ShutDown();
    void Close();
    static Client* GetInstance();
    void SendCommand(const std::string& command);
    void OnLoginSuccess();
    void OnLoginFail();
    void OnLogoutSuccess();
    void OnGetConversationSuccess();
    void OnGetUsersOnlineSuccess();
    void OnReceiveMessageSuccess();
    void OnAcceptJoinConversation();
    void OnLeftGroupChatSuccess();
    bool Check(); 

private:
    Client();
    void ReceiveMessages();
private:
    static Client* s_Instance;
    WSADATA m_WsaData;
    SOCKET m_ClientSocket = INVALID_SOCKET;
    uint32_t m_clientSocketId;
    struct sockaddr_in m_ServerAddr;
    
    std::thread m_ReceiveThread;
    uint32_t m_ServerPort;
    const char* m_ServerAddress;
    int iResult; // Result of recv()
    json responseData;

};
}