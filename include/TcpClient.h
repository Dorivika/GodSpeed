#pragma once
#include <string>
#include <functional>
#include <thread>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace networking {

class TcpClient {
public:
    TcpClient();
    ~TcpClient();

    bool connect(const std::string& serverIp, int port);
    bool send(const std::string& message);
    void disconnect();
    bool isConnected() const;

    using MessageHandler = std::function<void(const std::string&)>;
    void setMessageHandler(MessageHandler handler);

private:
    void receiveMessages();
    bool initializeSSL();
    void cleanupSSL();

    int clientSocket;
    bool connected;
    std::thread receiveThread;
    MessageHandler messageHandler;

    SSL_CTX* sslCtx;
    SSL* ssl;
};

} // namespace networking