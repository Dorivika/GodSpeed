#pragma once
#include <string>
#include <functional>
#include <thread>
#include <unordered_map>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef SOCKET socket_t;
    #define INVALID_SOCKET_VALUE INVALID_SOCKET
    #define SOCKET_ERROR_VALUE SOCKET_ERROR
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <errno.h>
    typedef int socket_t;
    #define INVALID_SOCKET_VALUE -1
    #define SOCKET_ERROR_VALUE -1
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
    void handleClient(socket_t clientSocket, int clientId);

    int port;
    bool running;
    socket_t serverSocket;
    std::thread acceptThread;
    std::unordered_map<socket_t, std::thread> clientThreads;
    MessageHandler messageHandler;
};

} // namespace networking