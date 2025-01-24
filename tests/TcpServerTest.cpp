#include <iostream>
#include <thread>
#include <chrono>
#include <cassert>
#include <vector>
#include "../include/TcpServer.h"
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef SOCKET socket_t;
    #define INVALID_SOCKET_VALUE INVALID_SOCKET
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
    typedef int socket_t;
    #define INVALID_SOCKET_VALUE -1
#endif

void clientFunction(const std::string& message, int clientId) {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for server to start

#ifdef _WIN32
    WSADATA wsaData;
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaResult != 0) {
        std::cerr << "Client " << clientId << " failed to initialize Winsock: " << wsaResult << std::endl;
        return;
    }
#endif

    socket_t clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET_VALUE) {
        std::cerr << "Client " << clientId << " failed to create socket" << std::endl;
#ifdef _WIN32
        WSACleanup();
#endif
        return;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    int result = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result < 0) {
        std::cerr << "Client " << clientId << " failed to connect to server: " << strerror(errno) << std::endl;
#ifdef _WIN32
        closesocket(clientSocket);
        WSACleanup();
#else
        close(clientSocket);
#endif
        return;
    }

    send(clientSocket, message.c_str(), static_cast<int>(message.size()), 0);

    char buffer[4096];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::cout << "Client " << clientId << " received from server: " << buffer << std::endl;
    } else {
        std::cerr << "Client " << clientId << " failed to receive message" << std::endl;
    }

#ifdef _WIN32
    closesocket(clientSocket);
    WSACleanup();
#else
    close(clientSocket);
#endif
}

void startServer() {
    networking::TcpServer server(8080);
    server.setMessageHandler([](int clientId, const std::string& message) {
        std::cout << "Server received from client " << clientId << ": " << message << std::endl;
    });

    if (!server.start()) {
        std::cerr << "Failed to start server" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(10)); // Keep the server running for a while
    server.stop();
}

int main() {
    std::thread serverThread(startServer);

    const int numClients = 5;
    const std::string message = "Hello from client";

    std::vector<std::thread> clientThreads;
    for (int i = 0; i < numClients; ++i) {
        clientThreads.emplace_back(clientFunction, message, i);
    }

    for (auto& thread : clientThreads) {
        thread.join();
    }

    serverThread.join();

    return 0;
}