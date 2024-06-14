#include "Core/Client/Client.h"
#include "ViewModel/UserViewModel.h"
#include "ViewModel/MessageViewModel.h"
#include "PCH.h"

namespace Piero {
Client* Client::s_Instance = nullptr;
Client::Client(): m_ServerPort(atoi(getenv("SERVER_PORT"))), m_ServerAddress(getenv("SERVER_ADDRESS")) {
    std::cout << "Connect server address: " << m_ServerAddress << std::endl;
    std::cout << "Server port:" << m_ServerPort << std::endl;

    int iResult = WSAStartup(MAKEWORD(2, 2), &m_WsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        ShutDown();
        return;
    }
    m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_ClientSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        ShutDown();
        return;
    }

    m_ServerAddr.sin_family = AF_INET;
    m_ServerAddr.sin_addr.s_addr = inet_addr(m_ServerAddress); // Địa chỉ IP của máy chủ trong mạng cục bộ
    m_ServerAddr.sin_port = htons(m_ServerPort);

    int connectResult = connect(m_ClientSocket, (struct sockaddr*)&m_ServerAddr, sizeof(m_ServerAddr));
    if (connectResult == SOCKET_ERROR) {
        std::cout << "connect failed: " << WSAGetLastError() << std::endl;
        closesocket(m_ClientSocket);
        ShutDown();
        return;
    }

    m_ReceiveThread = std::thread(&Client::ReceiveMessages, this);
    m_ReceiveThread.detach();
}


Client* Client::GetInstance() {
    if (s_Instance == nullptr) {
        s_Instance = new Client();
    }
    return s_Instance;
}

void Client::ReceiveMessages() {
    char recvbuf[DEFAULT_BUFLEN];
    do {
        iResult = recv(m_ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
        if (iResult > 0) {
            recvbuf[iResult] = '\0';
            responseData = json::parse(std::string(recvbuf));
            json jsonObject = json::parse(std::string(recvbuf));

            std::cout << responseData.dump(4) << std::endl;

            int statusCode = responseData["status"];
            std::cout << "StatusCode: " << statusCode << std::endl;
            std::cout << "Response message: " << std::string(responseData["message"]) << std::endl;

            switch (statusCode) {
                case StatusCode::LOGIN_SUCCESS:
                    OnLoginSuccess();
                    break;
                case StatusCode::LOGIN_FAILED_USER_NOT_EXIST: 
                    OnLoginFail();
                    break;
                case StatusCode::LOGOUT_SUCCESS:
                    OnLogoutSuccess();
                    break;
                case StatusCode::RECEIVE_MESSAGE_SUCCESS:
                    OnReceiveMessageSuccess();
                    break;
                case StatusCode::GET_USERS_ONLINE_SUCCESS:
                    OnGetUsersOnlineSuccess();
                    break;
                case StatusCode::GET_CONVERSATION_SUCCSESS:
                    OnGetConversationSuccess();
                    break;
                case StatusCode::CREATE_CONVERSATION_SUCCSESS: {
                    MessageViewModel::GetInstance()->Conversations.push_back(Conversation(jsonObject["conversation_id"], jsonObject["conversation_name"], jsonObject["is_group"]));
                    MessageViewModel::GetInstance()->CurrentConversation = MessageViewModel::GetInstance()->Conversations.back();
                    break;
                }
                case StatusCode::SEND_INVITATION_TO_JOIN_CONVERSATION_SUCCSESS: {
                    // show modal
                    if(responseData["user_id"] == UserViewModel::GetInstance()->CurrentUser.id) {
                        AcceptModal::isShowing = true;
                        AcceptModal::message = responseData["message"];
                        AcceptModal::conversationId = responseData["conversation_id"];
                    }
                    break;
                }
                case StatusCode::ACCEPT_JOIN_IN_CONVERSATION_SUCCSESS:
                    OnAcceptJoinConversation();
                    break;
                case StatusCode::GET_OR_CREATE_CONVERSATION_OF_TWO_USER_SUCCSESS: {
                    if(jsonObject["user_id_1"] == UserViewModel::GetInstance()->CurrentUser.id ) {
                        MessageViewModel::GetInstance()->CurrentConversation = Conversation(jsonObject["conversation_id"], jsonObject["conversation_name"], false);
                        if(!jsonObject["exist"]) {
                            MessageViewModel::GetInstance()->Conversations.push_back(MessageViewModel::GetInstance()->CurrentConversation);
                        }
                        MessageViewModel::GetInstance()->OnGetConversation(UserViewModel::GetInstance()->GetId(), jsonObject["conversation_id"]);
                    }
                    break;
                }
                case StatusCode::LEFT_GROUP_CHAT_SUCCSESS: {
                    OnLeftGroupChatSuccess();
                    break;
                }
                default:
                    break;
            }
        } 
        else if (iResult == 0) {
            std::cout << "Connection closed by server" << std::endl;
            break;
        } 
        else {
            std::cout << "recv failed: " << WSAGetLastError() << std::endl;
            break;
        }
    } while (iResult > 0);
}

void Client::SendCommand(const std::string& command) {
    send(m_ClientSocket, command.c_str(), command.size(), 0);
}
void Client::Close() {
    std::cout << "Close socket" << std::endl;
    closesocket(m_ClientSocket);
}
void Client::ShutDown() {
    
    std::cout << "Free Client Instance" << std::endl;
    Close();
    WSACleanup();
}
bool Client::Check() {
    if(m_clientSocketId == responseData["client_id"]) {
        return true;
    }
    return false;
}

Client::~Client() {}
    
}