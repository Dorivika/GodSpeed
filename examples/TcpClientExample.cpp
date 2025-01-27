#include <iostream>
#include <thread>
#include "../include/TcpClient.h"

void handleServerMessage(const std::string& message) {
    std::cout << "Client received: " << message << std::endl;
}

int main() {
    networking::TcpClient client;

    if (!client.connect("127.0.0.1", 8080)) {
        std::cerr << "Failed to connect to server" << std::endl;
        return 1;
    }

    client.setMessageHandler(handleServerMessage);

    std::string message = "Hello, Server!";
    if (!client.send(message)) {
        std::cerr << "Failed to send message" << std::endl;
        return 1;
    }

    std::cout << "Message sent: " << message << std::endl;

    // Keep the client running to receive messages
    std::this_thread::sleep_for(std::chrono::seconds(5));

    client.disconnect();
    std::cout << "Client disconnected" << std::endl;

    return 0;
}