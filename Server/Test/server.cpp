#include <iostream>
#include <WinSock2.h>
#include <vector>
#include <thread>
#include <algorithm>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

std::vector<SOCKET> clientSockets;

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

int main() {
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    // Create a socket for listening
    SOCKET listenSocket = INVALID_SOCKET;
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Setup server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(atoi(DEFAULT_PORT));

    // Bind socket
    iResult = bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        std::cout << "bind failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Listen on socket
    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cout << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    while (true) {
        // Accept a client socket
        SOCKET clientSocket;
        clientSocket = accept(listenSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "accept failed: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }

        // Add client socket to vector
        clientSockets.push_back(clientSocket);

        // Create a thread to handle client
        std::thread handlerThread(clientHandler, clientSocket);
        handlerThread.detach(); // Detach thread to run independently
    }

    closesocket(listenSocket);
    WSACleanup();

    return 0;
}