#pragma once
#include <string>
#include <functional>
#include <thread>

namespace networking {

class TcpClient {
public:
    TcpClient();
    ~TcpClient();

    bool connect(const std::string& serverIp, int port);
    void disconnect();
    bool send(const std::string& message);
    bool isConnected() const;

    using MessageHandler = std::function<void(const std::string& message)>;
    void setMessageHandler(MessageHandler handler);

private:
    void receiveMessages();
    
    int clientSocket;
    bool connected;
    std::thread receiveThread;
    MessageHandler messageHandler;
};

} // namespace networking