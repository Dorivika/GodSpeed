#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cassert>
#include "../include/TcpClient.h"
#include "../include/TcpServer.h"

std::mutex coutMutex;

void clientFunction(int clientId, const std::string& serverIp, int port) {
    networking::TcpClient client;
    assert(client.connect(serverIp, port));

    client.setMessageHandler([clientId](const std::string& message) {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Client " << clientId << " received: " << message << std::endl;
    });

    std::string message = "Hello from client " + std::to_string(clientId);
    assert(client.send(message));

    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for server response
    client.disconnect();
}

int main() {
    networking::TcpServer server(8080);

    server.setMessageHandler([](int clientId, const std::string& message) {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Server received from client " << clientId << ": " << message << std::endl;
    });

    if (!server.start()) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }
    std::cout << "Server started" << std::endl;

    const int numClients = 5;
    std::vector<std::thread> clientThreads;

    for (int i = 0; i < numClients; ++i) {
        clientThreads.emplace_back(clientFunction, i, "127.0.0.1", 8080);
    }

    for (auto& thread : clientThreads) {
        thread.join();
    }

    server.stop();
    return 0;
}