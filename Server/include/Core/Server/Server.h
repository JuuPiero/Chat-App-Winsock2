#pragma once

#define MAX_CLIENTS 1024
#define DEFAULT_BUFLEN 2048

#include "PCH.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
namespace Piero {

class Server {
public:
    enum Command {
        LOGIN = 0,
        LOGOUT = 1,
        SEND_MESSAGE = 2,
        
        GET_USERS_ONLINE = 4,
        GET_OR_CREATE_CONVERSATION_OF_TWO_USER = 5,
        INVITE_USER_TO_CONVERSATION = 6,


        GET_CONVERSATION = 20,
        CREATE_CONVERSATION = 21,

        ACCEPT_INVITE = 25,

        SEND_MESSAGE_TO_ALL,
        SEND_MESSAGE_TO_GROUP,
    };
    

public:
    static Server* GetInstance();
    void ShutDown();
    void Run();
    ~Server();
private:
    void ClientHandler(SOCKET clientSocket, uint32_t clientSocketId);
    // void ClientHandler(std::string clientSocketId);


    void OnUserLogin(uint32_t clientSocketId,  std::string username, std::string password);
    void OnUserLogout(SOCKET clientSocket, std::string& username, std::string& password);
    void OnUserGetUsersOnline(uint32_t clientSocketId, int userId);
    void OnUserSendMessage(uint32_t clientSocketId, int senderId, int conversationId, std::string& message, bool isQuitMessage);


    void OnUserGetConversation(SOCKET& clientSocket, int userId, int conversationId);
    void OnUserGetConversationOfTwoUsers(uint32_t clientSocketId, int userId1, int userId2);
    void OnUserInviteUserToConversation(SOCKET& clientSocket, int senderId, int userId, int conversationId);
    void OnUserAcceptInvite(SOCKET& clientSocket, int userId, int conversationId);
    void OnUserCreateConversation(SOCKET& clientSocket, int userId, std::string convsersationName);

private:
    Server();

    static Server* s_Insntance;
    WSADATA m_WsaData;
    SOCKET m_ListenSocket;
    sockaddr_in m_ServerAddr;
    uint32_t m_Port;
    // std::vector<SOCKET> m_ClientSockets;
    std::unordered_map<uint32_t, SOCKET> m_ClientSockets;
    std::unordered_map<std::string, uint32_t> m_UserOnlines; // username - id
    int iResult;
};

}



