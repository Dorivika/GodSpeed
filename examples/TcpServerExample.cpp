#include <iostream>
#include <thread>
#include "../include/TcpServer.h"

void handleClientMessage(int clientId, const std::string& message) {
    std::cout << "Server received from client " << clientId << ": " << message << std::endl;
}

int main() {
    networking::TcpServer server(8080);

    server.setMessageHandler(handleClientMessage);

    if (!server.start()) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }

    std::cout << "Server started on port 8080" << std::endl;

    // Keep the server running
    std::this_thread::sleep_for(std::chrono::minutes(10));

    server.stop();
    std::cout << "Server stopped" << std::endl;

    return 0;
}