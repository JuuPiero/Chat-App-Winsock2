#include "Core/Server/Server.h"
#include "spdlog/spdlog.h"
#include "Core/Model/Conversation.h"
namespace Piero {
Server* Server::s_Insntance = nullptr;

Server::Server(): m_Port(atoi(getenv("PORT"))) {
    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &m_WsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        ShutDown();
        return;
    }
    // Create a socket for listening
    m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_ListenSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        ShutDown();
        return;
    }

    // Setup server address
    m_ServerAddr.sin_family = AF_INET;
    m_ServerAddr.sin_addr.s_addr = INADDR_ANY; // cái này lắng nghe mọi máy từ mạng cục bộ
    m_ServerAddr.sin_port = htons(m_Port);

    // Bind socket
    iResult = bind(m_ListenSocket, (struct sockaddr*)&m_ServerAddr, sizeof(m_ServerAddr));
    if (iResult == SOCKET_ERROR) {
        std::cout << "bind failed: " << WSAGetLastError() << std::endl;
        ShutDown();
        return;
    }
    // Listen on socket
    iResult = listen(m_ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cout << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(m_ListenSocket);
        WSACleanup();
        return;
    }
    spdlog::info("Server listening on port: {}", m_Port);
}

Server* Server::GetInstance() {
    if (s_Insntance == nullptr) {
        s_Insntance = new Server();
    }
    return s_Insntance;
}
void Server::ShutDown() {
    closesocket(m_ListenSocket);
    WSACleanup();
    // m_UserOnlines.clear();
    m_ClientSockets.clear();
    delete s_Insntance;
    s_Insntance = nullptr;
}

void Server::Run() {
    // Accept a client socket
    SOCKET clientSocket;
    clientSocket = accept(m_ListenSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "accept failed: " << WSAGetLastError() << std::endl;
        closesocket(m_ListenSocket);
        WSACleanup();
        return;
    }
    uint32_t id = GetId();
    // Add client socket to vector
    // m_ClientSockets.push_back(clientSocket);
    m_ClientSockets[id] = clientSocket;
    // Create a thread to handle client
    std::thread handlerThread(&Server::ClientHandler, this, clientSocket, id);
    handlerThread.detach(); // Detach thread to run independently
}

void Server::ClientHandler(SOCKET clientSocket, uint32_t clientSocketId) {
    
    // SOCKET clientSocket = m_ClientSockets[clientSocketId];
  
    char recvbuf[DEFAULT_BUFLEN];
    spdlog::info("A client connected to server: {}",  clientSocket);

    do {
        iResult = recv(clientSocket, recvbuf, DEFAULT_BUFLEN, 0);
        if (iResult > 0) {
            std::cout << "A client connected to server: " << clientSocket << std::endl;
            recvbuf[iResult] = '\0';
            //parse data from client
            std::istringstream iss(recvbuf);

            // requestData = json::parse(std::string(recvbuf));
            json requestData = json::parse(std::string(recvbuf));
            json jsonObject = json::parse(std::string(recvbuf));

            std::cout << "message from client: " << std::endl;
            std::cout << requestData.dump(4) << std::endl;

            int command = requestData["command"];

            if(command == Command::LOGIN) {
                std::string username = requestData["data"]["username"];
                std::string password = requestData["data"]["password"];
                OnUserLogin(clientSocketId, username, password);
            }
            else if(command == Command::LOGOUT) {
                std::string username = jsonObject["data"]["username"];
                std::string password = jsonObject["data"]["password"];
                OnUserLogout(clientSocket, username, password);
            }

            else if(command == Command::SEND_MESSAGE) {
                std::cout << "Thuc hien chuc nang gui tin nhan TRONG 1 HOI THOAI" << std::endl;
                int senderId = jsonObject["sender_id"];
                int conversationId = jsonObject["conversation_id"];
                std::string message = jsonObject["message"];
                bool isQuitMessage = jsonObject["is_quit_message"];
                // int receiverId = jsonObject["receiver_id"];
                OnUserSendMessage(clientSocketId, senderId, conversationId, message, isQuitMessage);
            }
            else if(command == Command::GET_USERS_ONLINE) {
                int userId = jsonObject["user_id"];
                OnUserGetUsersOnline(clientSocketId, userId);
            }

            else if(command == Command::CREATE_CONVERSATION) {
                int userId = requestData["user_id"];
                std::string name = requestData["conversation_name"];
                OnUserCreateConversation(clientSocket, userId, name);
            }

            else if(command == Command::GET_CONVERSATION) {
                int userId = jsonObject["user_id"];
                int conversationId = jsonObject["conversation_id"];
                OnUserGetConversation(clientSocket, userId, conversationId);
            }

            else if(command == Command::INVITE_USER_TO_CONVERSATION) {
                OnUserInviteUserToConversation(clientSocket, requestData["sender_id"], requestData["user_id"], requestData["conversation_id"]);
            }
            else if(command == Command::ACCEPT_INVITE) {
                OnUserAcceptInvite(clientSocket, requestData["user_id"], requestData["conversation_id"]);
            }

            //
            else if(command == Command::GET_OR_CREATE_CONVERSATION_OF_TWO_USER) {
                OnUserGetConversationOfTwoUsers(clientSocketId, jsonObject["user_id_1"], jsonObject["user_id_2"]);
            }
           
        } else if (iResult == 0) {
            std::cout << "Connection closed by client" << std::endl;
        } else {
            spdlog::error("A client disconnected : {}", WSAGetLastError());
        }
    } while (iResult > 0);

    closesocket(clientSocket);
    // Remove socket from vector
    m_ClientSockets.erase(clientSocketId);
    // auto it = std::find(m_ClientSockets.begin(), m_ClientSockets.end(), clientSocket);
    // if (it != m_ClientSockets.end()) {
    //     m_ClientSockets.erase(it);
    // }
}

Server::~Server() {}
}