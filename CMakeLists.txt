cmake_minimum_required(VERSION 3.10)
project(godspeed)

set(CMAKE_CXX_STANDARD 17)

# Create library from sources, including TcpServer
add_library(godspeed
    src/TcpServer.cpp
    src/TcpClient.cpp
    src/UdpSocket.cpp
    src/NetworkUtils.cpp
)

# Include your headers
target_include_directories(godspeed
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Windows-specific libraries
if(WIN32)
    target_link_libraries(godspeed ws2_32)
endif()

# Executables
add_executable(NetworkUtilsTest tests/NetworkUtilsTest.cpp)
add_executable(TcpServerTest tests/TcpServerTest.cpp)
add_executable(TcpClientTest tests/TcpClientTest.cpp)
add_executable(UdpSocketTest tests/UdpSocketTest.cpp)

# Link them against the godspeed library
target_link_libraries(NetworkUtilsTest godspeed)
target_link_libraries(TcpServerTest  godspeed)
target_link_libraries(TcpClientTest  godspeed)
target_link_libraries(UdpSocketTest  godspeed)