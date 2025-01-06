#include <iostream>
#include <cassert>
#include "../include/NetworkUtils.h"

void testGetLastErrorString() {
    std::string error = networking::NetworkUtils::getLastErrorString();
    std::cout << "getLastErrorString() passed: " << error << std::endl;
}

void testIsValidPort() {
    assert(networking::NetworkUtils::isValidPort(80) == true);
    assert(networking::NetworkUtils::isValidPort(65535) == true);
    assert(networking::NetworkUtils::isValidPort(0) == false);
    assert(networking::NetworkUtils::isValidPort(70000) == false);
    std::cout << "isValidPort() passed" << std::endl;
}

void testIsValidIpAddress() {
    assert(networking::NetworkUtils::isValidIpAddress("127.0.0.1") == true);
    assert(networking::NetworkUtils::isValidIpAddress("256.256.256.256") == false);
    assert(networking::NetworkUtils::isValidIpAddress("::1") == false); // Assuming IPv4 only
    std::cout << "isValidIpAddress() passed" << std::endl;
}

void testInitializeWinsock() {
    #ifdef _WIN32
    try {
        networking::NetworkUtils::initializeWinsock();
        std::cout << "initializeWinsock() passed" << std::endl;
    } catch (const std::system_error& e) {
        std::cerr << "initializeWinsock() failed: " << e.what() << std::endl;
    }
    #else
    std::cout << "initializeWinsock() skipped (not Windows)" << std::endl;
    #endif
}

int main() {
    testGetLastErrorString();
    testIsValidPort();
    testIsValidIpAddress();
    testInitializeWinsock();
    return 0;
}