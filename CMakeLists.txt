cmake_minimum_required(VERSION 3.10)
project(NetworkingLibrary)

set(CMAKE_CXX_STANDARD 17)

# Add source files
add_library(NetworkingLibrary
    src/TcpServer.cpp
    src/TcpClient.cpp
    src/UdpSocket.cpp
    src/NetworkUtils.cpp
)

# Include directories
target_include_directories(NetworkingLibrary PUBLIC include)

# Link libraries for Windows
if (WIN32)
    target_link_libraries(NetworkingLibrary ws2_32)
endif()

# Add test executables
add_executable(NetworkUtilsTest tests/NetworkUtilsTest.cpp)
add_executable(TcpServerTest tests/TcpServerTest.cpp)

# Link the library to the test executables
target_link_libraries(NetworkUtilsTest NetworkingLibrary)
target_link_libraries(TcpServerTest NetworkingLibrary)