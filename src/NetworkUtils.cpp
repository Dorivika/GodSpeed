#include "NetworkUtils.h"
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
    std::string NetworkUtils::getLastErrorString() {
        #ifdef _WIN32
            char* errorMessage = nullptr;
            FormatMessageA (
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr,
                WSAGetLastError(),
                0,
                (LPSTR)&errorMessage,
                0,
                nullptr
            );
        std::string error(errorMessage);
        LocalFree(errorMessage);
        return error;
        #else
            return std::string(strerror(errno));
        #endif
    }
    
    bool NetworkUtils::isValidPort(int port) {
        return port > 0 && port < 65536;
    }

    bool NetworkUtils::isValidIpAddress(const std::string& ipAddress) {
        struct sockaddr_in sa;
        return inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr)) != 0;
    }

    void NetworkUtils::initializeWinsock() {
        #ifdef _WIN32
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                throw std::system_error(WSAGetLastError(), std::system_category(), "WSAStartup failed");
            }
        #endif
    }
}