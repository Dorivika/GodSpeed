// include/TcpServer.h
#pragma once
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <unordered_map>

namespace networking {

class TcpServer {
public:
    TcpServer(int port);
    ~TcpServer();

    bool start();
    void stop();
    bool isRunning() const;
    
    using MessageHandler = std::function<void(int clientId, const std::string& message)>;
    void setMessageHandler(MessageHandler handler);

private:
    void acceptClients();
    void handleClient(int clientSocket, int clientId);
    
    int serverSocket;
    int port;
    bool running;
    std::thread acceptThread;
    std::mutex clientsMutex;
    std::unordered_map<int, std::thread> clientThreads;
    MessageHandler messageHandler;
};