#include "TcpServer.h"
#include "NetworkUtils.h"
#include <iostream>
#include <cstring>
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

TcpServer::TcpServer(int port) : port(port), running(false), serverSocket(INVALID_SOCKET_VALUE) {
#ifdef _WIN32
    NetworkUtils::initializeWinsock();
#endif
}

TcpServer::~TcpServer() {
    stop();
}

bool TcpServer::start() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET_VALUE) {
        return false;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR_VALUE) {
        return false;
    }

    if (listen(serverSocket, 5) == SOCKET_ERROR_VALUE) {
        return false;
    }

    running = true;
    acceptThread = std::thread(&TcpServer::acceptClients, this);
    return true;
}

void TcpServer::stop() {
    running = false;
#ifdef _WIN32
    closesocket(serverSocket);
#else
    close(serverSocket);
#endif

    if (acceptThread.joinable()) {
        acceptThread.join();
    }

    std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto& client : clientThreads) {
#ifdef _WIN32
        closesocket(client.first);
#else
        close(client.first);
#endif
        if (client.second.joinable()) {
            client.second.join();
        }
    }
    clientThreads.clear();
}

void TcpServer::acceptClients() {
    while (running) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        
        socket_t clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket == INVALID_SOCKET_VALUE) {
            if (!running) {
                break; // Exit the loop if the server is stopping
            }
            continue;
        }

        std::lock_guard<std::mutex> lock(clientsMutex);
        clientThreads[clientSocket] = std::thread(&TcpServer::handleClient, this, clientSocket, static_cast<int>(clientSocket));
    }
}

void TcpServer::handleClient(socket_t clientSocket, int clientId) {
    char buffer[4096];
    while (running) {
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            break;
        }
        
        buffer[bytesRead] = '\0';
        if (messageHandler) {
            messageHandler(clientId, std::string(buffer));
        }
    }

    std::lock_guard<std::mutex> lock(clientsMutex);
    clientThreads.erase(clientSocket);
#ifdef _WIN32
    closesocket(clientSocket);
#else
    close(clientSocket);
#endif
}

void TcpServer::setMessageHandler(MessageHandler handler) {
    messageHandler = std::move(handler);
}

bool TcpServer::isRunning() const {
    return running;
}

} // namespace networking