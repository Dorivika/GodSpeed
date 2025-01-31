cmake_minimum_required(VERSION 3.10)
project(godspeed VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Create library from sources
add_library(godspeed
    src/TcpServer.cpp
    src/TcpClient.cpp
    src/UdpSocket.cpp
    src/NetworkUtils.cpp
)

# Include directories
target_include_directories(godspeed PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${OPENSSL_INCLUDE_DIR}
)

# Link libraries for Windows
if(WIN32)
    target_link_libraries(godspeed ws2_32 OpenSSL::SSL OpenSSL::Crypto)
else()
    target_link_libraries(godspeed OpenSSL::SSL OpenSSL::Crypto)
endif()

# Installation rules
install(TARGETS godspeed
    EXPORT godspeedTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

# Export the targets to a script
install(EXPORT godspeedTargets
    FILE godspeedTargets.cmake
    NAMESPACE godspeed::
    DESTINATION lib/cmake/godspeed
)

# Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/godspeedConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Create a Config.cmake file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/godspeedConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/godspeedConfig.cmake"
    INSTALL_DESTINATION lib/cmake/godspeed
)

# Install the Config.cmake and ConfigVersion.cmake files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/godspeedConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/godspeedConfigVersion.cmake"
    DESTINATION lib/cmake/godspeed
)

# Export the package for use from the build tree
export(EXPORT godspeedTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/godspeedTargets.cmake"
    NAMESPACE godspeed::
)

# Register the package in the user's package registry
export(PACKAGE godspeed)

# Executables
add_executable(NetworkUtilsTest tests/NetworkUtilsTest.cpp)
add_executable(TcpServerTest tests/TcpServerTest.cpp)
add_executable(TcpClientTest tests/TcpClientTest.cpp)
add_executable(UdpSocketTest tests/UdpSocketTest.cpp)

# Link them against the godspeed library
target_link_libraries(NetworkUtilsTest godspeed)
target_link_libraries(TcpServerTest godspeed)
target_link_libraries(TcpClientTest godspeed)
target_link_libraries(UdpSocketTest godspeed)