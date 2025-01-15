#pragma once
#include <string>

namespace networking {

class UdpSocket {
public:
    UdpSocket();
    ~UdpSocket();

    bool bind(int port);
    bool sendTo(const std::string& message, const std::string& ip, int port);
    bool receive(std::string& message, std::string& senderIp, int& senderPort);

private:
    int socket;
    bool bound;
};

} // namespace networking