#pragma once
#include <string>

#ifdef _WIN32
    #include <winsock2.h>
    typedef SOCKET socket_t;
    #define INVALID_SOCKET_VALUE INVALID_SOCKET
    #define SOCKET_ERROR_VALUE SOCKET_ERROR
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <errno.h>
    typedef int socket_t;
    #define INVALID_SOCKET_VALUE -1
    #define SOCKET_ERROR_VALUE -1
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
    socket_t socket;
    bool bound;
};

} // namespace networking