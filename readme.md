# GodSpeed

## Overview
**GodSpeed** is a C++ library designed to simplify the implementation of networking functionalities such as TCP and UDP communication on both Windows and Linux.

## Features
- **TCP Server**: Handle multiple clients with a robust and scalable architecture.
- **TCP Client**: Connect to remote servers and manage communication seamlessly.
- **UDP Socket**: Send and receive datagrams with ease.
- **Event-Driven Architecture**: Efficiently manage events and I/O operations.
- **Multi-threading Support**: Built-in support for handling multiple clients concurrently.

## Requirements
- C++17 or later
- CMake (3.15 or later)

## Building the Project
1. Clone the repository:
   ```bash
   git clone https://github.com/Dorivika/GodSpeed.git
   cd GodSpeed
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
g++ -std=c++17 -Iinclude -o TcpServerExample examples/TcpServerExample.cpp src/TcpServer.cpp src/NetworkUtils.cpp -lws2_32 -lpthread
```
This will start a simple TCP server.

```bash
g++ -std=c++17 -Iinclude -o TcpClientExample examples/TcpClientExample.cpp src/TcpClient.cpp src/NetworkUtils.cpp -lws2_32 -lpthread
```
This will start a simple TCP client.


## Future Extensions
- Add TLS support using OpenSSL.
- Implement a thread pool for efficient resource management.
- Create a higher-level API for simplified usage.
- Add support for non-blocking I/O.
