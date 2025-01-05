#pragma once
#include <string>
#include <system_error>

namespace networking {
    class NetworkUtils { 
        public:
            static std::string getLastErrorString();
            static bool isValidPort(int port);
            static bool isValidIpAddress(const std::string& ipAddress);
            static void initializeWinsock(); //for windows only
    };
}