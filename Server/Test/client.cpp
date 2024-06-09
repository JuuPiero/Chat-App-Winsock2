#include <iostream>
#include <WinSock2.h>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

void receiveMessages(SOCKET connectSocket) {
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;

    do {
        iResult = recv(connectSocket, recvbuf, DEFAULT_BUFLEN, 0);
        if (iResult > 0) {
            recvbuf[iResult] = '\0';
            std::cout << "Message from server: " << recvbuf << std::endl;
        } else if (iResult == 0) {
            std::cout << "Connection closed by server" << std::endl;
        } else {
            std::cout << "recv failed: " << WSAGetLastError() << std::endl;
        }
    } while (iResult > 0);
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

    // Create a socket for connecting to server
    SOCKET connectSocket = INVALID_SOCKET;
    connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connectSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Setup server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Địa chỉ IP của máy chủ
    serverAddr.sin_port = htons(atoi(DEFAULT_PORT)); // Số cổng của máy chủ

    // Connect to server
    iResult = connect(connectSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        std::cout << "Unable to connect to server: " << WSAGetLastError() << std::endl;
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }

    // Create a thread to receive messages from server
    std::thread receiveThread(receiveMessages, connectSocket);
    receiveThread.detach(); // Detach thread to run independently

    // Send messages to server
    char sendbuf[DEFAULT_BUFLEN];
    int sendResult;

    do {
        std::cout << "Enter message: ";
        std::cin.getline(sendbuf, DEFAULT_BUFLEN);

        sendResult = send(connectSocket, sendbuf, (int)strlen(sendbuf), 0);
        if (sendResult == SOCKET_ERROR) {
            std::cout << "send failed: " << WSAGetLastError() << std::endl;
            closesocket(connectSocket);
            WSACleanup();
            return 1;
        }
    } while (true);

    // Cleanup
    closesocket(connectSocket);
    WSACleanup();

    return 0;
}