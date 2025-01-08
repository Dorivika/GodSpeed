#include <iostream>
#include <thread>
#include <chrono>
#include <cassert>
#include "../include/TcpServer.h"
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif

void clientFunction(const std::string& message) {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for server to start

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    assert(clientSocket >= 0);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    int result = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    assert(result >= 0);

    send(clientSocket, message.c_str(), message.size(), 0);

    char buffer[4096];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    buffer[bytesRead] = '\0';

    std::cout << "Received from server: " << buffer << std::endl;

#ifdef _WIN32
    closesocket(clientSocket);
#else
    close(clientSocket);
#endif
}

int main() {
    networking::TcpServer server(8080);

    server.setMessageHandler([](int clientId, const std::string& message) {
        std::cout << "Received from client " << clientId << ": " << message << std::endl;
    });

    assert(server.start());
    std::cout << "Server started" << std::endl;

    std::thread clientThread(clientFunction, "Hello, Server!");

    clientThread.join();
    server.stop();
    std::cout << "Server stopped" << std::endl;

    return 0;
}