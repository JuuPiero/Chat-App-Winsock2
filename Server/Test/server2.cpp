#include <iostream>
#include <WinSock2.h>
#include <vector>
#include <thread>
#include <algorithm>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

class Server {
public:
    Server() {
        // Initialize Winsock
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            std::cout << "WSAStartup failed: " << iResult << std::endl;
            return;
        }

        // Create a socket for listening
        listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listenSocket == INVALID_SOCKET) {
            std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
            WSACleanup();
            return;
        }

        // Setup server address
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(atoi(DEFAULT_PORT));

        // Bind socket
        iResult = bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if (iResult == SOCKET_ERROR) {
            std::cout << "bind failed: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return;
        }

        // Listen on socket
        iResult = listen(listenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            std::cout << "listen failed: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return;
        }
    }

    ~Server() {
        closesocket(listenSocket);
        WSACleanup();
    }

    void start() {
        while (true) {
            // Accept a client socket
            SOCKET clientSocket;
            clientSocket = accept(listenSocket, nullptr, nullptr);
            if (clientSocket == INVALID_SOCKET) {
                std::cout << "accept failed: " << WSAGetLastError() << std::endl;
                closesocket(listenSocket);
                WSACleanup();
                return;
            }

            // Add client socket to vector
            clientSockets.push_back(clientSocket);

            // Create a thread to handle client
            std::thread handlerThread(&Server::clientHandler, this, clientSocket);
            handlerThread.detach(); // Detach thread to run independently
        }
    }

private:
    WSADATA wsaData;
    SOCKET listenSocket;
    std::vector<SOCKET> clientSockets;
    sockaddr_in serverAddr;

    void clientHandler(SOCKET clientSocket) {
        char recvbuf[DEFAULT_BUFLEN];
        int iResult;

        do {
            iResult = recv(clientSocket, recvbuf, DEFAULT_BUFLEN, 0);
            if (iResult > 0) {
                recvbuf[iResult] = '\0';
                std::cout << "Message from client: " << recvbuf << std::endl;

                // Broadcast message to all clients
                for (SOCKET& sock : clientSockets) {
                    if (sock != clientSocket) {
                        iResult = send(sock, recvbuf, iResult, 0);
                        if (iResult == SOCKET_ERROR) {
                            std::cout << "send failed: " << WSAGetLastError() << std::endl;
                        }
                    }
                }
            } else if (iResult == 0) {
                std::cout << "Connection closed by client" << std::endl;
            } else {
                std::cout << "recv failed: " << WSAGetLastError() << std::endl;
            }
        } while (iResult > 0);

        closesocket(clientSocket);

        // Remove socket from vector
        auto it = std::find(clientSockets.begin(), clientSockets.end(), clientSocket);
        if (it != clientSockets.end()) {
            clientSockets.erase(it);
        }
    }
};

int main() {
    Server server;
    server.start();

    return 0;
}