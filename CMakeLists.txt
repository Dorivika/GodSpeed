cmake_minimum_required(VERSION 3.10)
project(godspeed)

set(CMAKE_CXX_STANDARD 17)

# Add source files
add_library(godspeed
    src/TcpServer.cpp
    src/TcpClient.cpp
    src/UdpSocket.cpp
    src/NetworkUtils.cpp
)

# Include directories
target_include_directories(godspeed PUBLIC include)

# Link libraries for Windows
if (WIN32)
    target_link_libraries(godspeed ws2_32)
endif()

# Add test executables
add_executable(NetworkUtilsTest tests/NetworkUtilsTest.cpp)
add_executable(TcpServerTest tests/TcpServerTest.cpp)
add_executable(TcpClientTest tests/TcpClientTest.cpp)
add_executable(UdpSocketTest tests/UdpSocketTest.cpp)

# Link the library to the test executables
target_link_libraries(NetworkUtilsTest godspeed)
target_link_libraries(TcpServerTest godspeed)
target_link_libraries(TcpClientTest godspeed)
target_link_libraries(UdpSocketTest godspeed)