#include "TcpClient.h"
#include "NetworkUtils.h"
#include <iostream>
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <errno.h>
#endif

namespace networking {

TcpClient::TcpClient() : clientSocket(-1), connected(false) {
#ifdef _WIN32
    NetworkUtils::initializeWinsock();
#endif
}

TcpClient::~TcpClient() {
    disconnect();
}

bool TcpClient::connect(const std::string& serverIp, int port) {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        return false;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr) <= 0) {
        return false;
    }

    if (::connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        return false;
    }

    connected = true;
    receiveThread = std::thread(&TcpClient::receiveMessages, this);
    return true;
}

void TcpClient::disconnect() {
    connected = false;
#ifdef _WIN32
    closesocket(clientSocket);
#else
    close(clientSocket);
#endif

    if (receiveThread.joinable()) {
        receiveThread.join();
    }
}

bool TcpClient::send(const std::string& message) {
    if (!connected) return false;
    return ::send(clientSocket, message.c_str(), message.length(), 0) >= 0;
}

void TcpClient::receiveMessages() {
    char buffer[4096];
    while (connected) {
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            break;
        }
        
        buffer[bytesRead] = '\0';
        if (messageHandler) {
            messageHandler(std::string(buffer));
        }
    }
}

void TcpClient::setMessageHandler(MessageHandler handler) {
    messageHandler = std::move(handler);
}

bool TcpClient::isConnected() const {
    return connected;
}

}