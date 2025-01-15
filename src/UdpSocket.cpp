#include "UdpSocket.h"
#include "NetworkUtils.h"
#include <iostream>
#include <cstring>
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <errno.h>
#endif
namespace networking {

UdpSocket::UdpSocket() : socket(-1), bound(false) {
#ifdef _WIN32
    NetworkUtils::initializeWinsock();
#endif
}

UdpSocket::~UdpSocket() {
    if (socket >= 0) {
#ifdef _WIN32
        closesocket(socket);
#else
        close(socket);
#endif
    }
}

bool UdpSocket::bind(int port) {
    socket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (socket < 0) {
        return false;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (::bind(socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        return false;
    }

    bound = true;
    return true;
}

bool UdpSocket::sendTo(const std::string& message, const std::string& ip, int port) {
    if (socket < 0) return false;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
        return false;
    }

    return sendto(socket, message.c_str(), message.length(), 0,
                 (struct sockaddr*)&addr, sizeof(addr)) >= 0;
}

bool UdpSocket::receive(std::string& message, std::string& senderIp, int& senderPort) {
    if (!bound) return false;

    char buffer[4096];
    struct sockaddr_in senderAddr;
    socklen_t senderLen = sizeof(senderAddr);

    int bytesRead = recvfrom(socket, buffer, sizeof(buffer) - 1, 0,
                            (struct sockaddr*)&senderAddr, &senderLen);
    
    if (bytesRead < 0) {
        return false;
    }

    buffer[bytesRead] = '\0';
    message = std::string(buffer);
    
    char ipBuffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(senderAddr.sin_addr), ipBuffer, INET_ADDRSTRLEN);
    senderIp = std::string(ipBuffer);
    senderPort = ntohs(senderAddr.sin_port);

    return true;
}

} // namespace networking