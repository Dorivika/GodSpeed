#pragma once
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <unordered_map>

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <sys/socket.h>
#endif

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
    void handleClient(SOCKET clientSocket, int clientId);
    
    SOCKET serverSocket;
    int port;
    bool running;
    std::thread acceptThread;
    std::mutex clientsMutex;
    std::unordered_map<SOCKET, std::thread> clientThreads;
    MessageHandler messageHandler;
};

} // namespace networking