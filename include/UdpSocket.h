#pragma once
#include <string>

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <sys/socket.h>
#endif

namespace networking {

class UdpSocket {
public:
    UdpSocket();
    ~UdpSocket();

    bool bind(int port);
    bool sendTo(const std::string& message, const std::string& ip, int port);
    bool receive(std::string& message, std::string& senderIp, int& senderPort);

private:
    SOCKET socket;
    bool bound;
};

} // namespace networking