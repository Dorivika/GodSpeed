#include "UdpSocket.h"
#include "NetworkUtils.h"
#include <iostream>
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <cstring>
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <errno.h>
#endif

namespace networking {

UdpSocket::UdpSocket() : socket(INVALID_SOCKET), bound(false) {
#ifdef _WIN32
    NetworkUtils::initializeWinsock();
#endif
    socket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (socket == INVALID_SOCKET) {
#ifdef _WIN32
        char errorMsg[256];
        strerror_s(errorMsg, sizeof(errorMsg), errno);
        std::cerr << "Failed to create socket: " << errorMsg << std::endl;
#else
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
#endif
    }
}

UdpSocket::~UdpSocket() {
    if (socket != INVALID_SOCKET) {
#ifdef _WIN32
        closesocket(socket);
#else
        close(socket);
#endif
    }
}

bool UdpSocket::bind(int port) {
    if (socket == INVALID_SOCKET) {
        std::cerr << "Socket not created" << std::endl;
        return false;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (::bind(socket, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
#ifdef _WIN32
        char errorMsg[256];
        strerror_s(errorMsg, sizeof(errorMsg), errno);
        std::cerr << "Failed to bind socket: " << errorMsg << std::endl;
#else
        std::cerr << "Failed to bind socket: " << strerror(errno) << std::endl;
#endif
        return false;
    }

    bound = true;
    return true;
}

bool UdpSocket::sendTo(const std::string& message, const std::string& ip, int port) {
    if (socket == INVALID_SOCKET) {
        std::cerr << "Socket not created" << std::endl;
        return false;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
        std::cerr << "Invalid IP address: " << ip << std::endl;
        return false;
    }

    int result = sendto(socket, message.c_str(), static_cast<int>(message.size()), 0, (struct sockaddr*)&addr, sizeof(addr));
    if (result == SOCKET_ERROR) {
#ifdef _WIN32
        char errorMsg[256];
        strerror_s(errorMsg, sizeof(errorMsg), errno);
        std::cerr << "Failed to send message: " << errorMsg << std::endl;
#else
        std::cerr << "Failed to send message: " << strerror(errno) << std::endl;
#endif
        return false;
    }

    return true;
}

bool UdpSocket::receive(std::string& message, std::string& senderIp, int& senderPort) {
    if (socket == INVALID_SOCKET) {
        std::cerr << "Socket not created" << std::endl;
        return false;
    }

    char buffer[4096];
    struct sockaddr_in senderAddr;
    socklen_t senderAddrLen = sizeof(senderAddr);

    int bytesRead = recvfrom(socket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&senderAddr, &senderAddrLen);
    if (bytesRead == SOCKET_ERROR) {
#ifdef _WIN32
        char errorMsg[256];
        strerror_s(errorMsg, sizeof(errorMsg), errno);
        std::cerr << "Failed to receive message: " << errorMsg << std::endl;
#else
        std::cerr << "Failed to receive message: " << strerror(errno) << std::endl;
#endif
        return false;
    }

    buffer[bytesRead] = '\0';
    message = std::string(buffer);

    char ipBuffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &senderAddr.sin_addr, ipBuffer, sizeof(ipBuffer));
    senderIp = std::string(ipBuffer);
    senderPort = ntohs(senderAddr.sin_port);

    return true;
}

} // namespace networking