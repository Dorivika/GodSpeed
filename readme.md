# Networking Library

## Overview
**GodSpeed** is a C++ library designed to simplify the implementation of networking functionalities such as TCP and UDP communication. The library is designed with modularity, scalability, and adherence to professional C++ project norms, making it a great choice for both learning and real-world applications.

## Features
- **TCP Server**: Handle multiple clients with a robust and scalable architecture.
- **TCP Client**: Connect to remote servers and manage communication seamlessly.
- **UDP Socket**: Send and receive datagrams with ease.
- **Event-Driven Architecture**: Efficiently manage events and I/O operations.
- **Multi-threading Support**: Built-in support for handling multiple clients concurrently.
- **Extensibility**: Add features like TLS for secure communication.

## Project Structure
```
NetworkingLibrary/
├── include/          # Public headers
│   ├── TcpServer.h
│   ├── TcpClient.h
│   ├── UdpSocket.h
│   ├── NetworkUtils.h
├── src/              # Source files
│   ├── TcpServer.cpp
│   ├── TcpClient.cpp
│   ├── UdpSocket.cpp
│   ├── NetworkUtils.cpp
├── tests/            # Unit tests
│   ├── TcpServerTest.cpp
│   ├── TcpClientTest.cpp
│   ├── UdpSocketTest.cpp
├── examples/         # Example programs
│   ├── TcpExample.cpp
│   ├── UdpExample.cpp
├── CMakeLists.txt    # Build configuration
├── README.md         # Documentation
├── LICENSE           # License file
└── .gitignore        # Git ignore file
```

## Requirements
- C++17 or later
- CMake (3.15 or later)
- Optional:
  - Boost.Asio or standalone Asio for advanced networking features
  - OpenSSL for TLS support
  - Google Test for unit testing

## Building the Project
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd NetworkingLibrary
   ```
2. Create a build directory and navigate to it:
   ```bash
   mkdir build && cd build
   ```
3. Configure the project with CMake:
   ```bash
   cmake ..
   ```
4. Build the library and examples:
   ```bash
   make
   ```

## Running the Examples
After building the project, you can run the example programs located in the `examples` directory.

### Running the TCP Example
```bash
./examples/TcpExample
```
This will start a simple TCP server.

### Running the UDP Example
```bash
./examples/UdpExample
```
This will demonstrate basic UDP communication.

## Writing Tests
Unit tests are located in the `tests` directory and use the Google Test framework.

To build and run tests:
```bash
make test
./tests/YourTestExecutable
```

## Future Extensions
- Add TLS support using OpenSSL.
- Implement a thread pool for efficient resource management.
- Create a higher-level API for simplified usage.
- Add support for non-blocking I/O.
