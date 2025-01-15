#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cassert>
#include "../include/UdpSocket.h"

std::mutex coutMutex;

void senderFunction(int senderId, const std::string& message, const std::string& ip, int port) {
    networking::UdpSocket senderSocket;
    if (!senderSocket.sendTo(message, ip, port)) {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cerr << "Sender " << senderId << " failed to send: " << message << std::endl;
        return;
    }

    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "Sender " << senderId << " sent: " << message << std::endl;
}

void receiverFunction(int receiverId, int port) {
    networking::UdpSocket receiverSocket;
    if (!receiverSocket.bind(port)) {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cerr << "Receiver " << receiverId << " failed to bind to port " << port << std::endl;
        return;
    }

    std::string message;
    std::string senderIp;
    int senderPort;

    if (!receiverSocket.receive(message, senderIp, senderPort)) {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cerr << "Receiver " << receiverId << " failed to receive message" << std::endl;
        return;
    }

    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "Receiver " << receiverId << " received: " << message << " from " << senderIp << ":" << senderPort << std::endl;
}

int main() {
    const int numSenders = 5;
    const int numReceivers = 5;
    const int basePort = 8080;
    const std::string ip = "127.0.0.1";

    std::vector<std::thread> senderThreads;
    std::vector<std::thread> receiverThreads;

    for (int i = 0; i < numReceivers; ++i) {
        receiverThreads.emplace_back(receiverFunction, i, basePort + i);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for receivers to bind

    for (int i = 0; i < numSenders; ++i) {
        senderThreads.emplace_back(senderFunction, i, "Hello from sender " + std::to_string(i), ip, basePort + i);
    }

    for (auto& thread : senderThreads) {
        thread.join();
    }

    for (auto& thread : receiverThreads) {
        thread.join();
    }

    return 0;
}