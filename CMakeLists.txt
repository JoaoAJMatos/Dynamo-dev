# Set cmake version and project name
cmake_minimum_required(VERSION 3.21)
project(dev_dynamo)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
add_subdirectory(servers)

add_executable(dev_dynamo main.cpp src/networking/server/servers.h src/networking/protocols/HTTP.cpp src/networking/protocols/HTTP.h src/networking/networking.h src/system/system.h src/networking/sockets/common/Socket.cpp src/networking/sockets/common/Socket.h src/networking/sockets/sockets.h src/networking/sockets/SocketBinder.cpp src/networking/sockets/SocketBinder.h src/networking/sockets/SocketConnector.cpp src/networking/sockets/SocketConnector.h src/networking/server/BasicServer.cpp src/networking/server/BasicServer.h src/networking/sockets/SocketListener.cpp src/networking/sockets/SocketListener.h src/system/time/Time.cpp src/system/time/Time.h src/system/threading/ThreadPool.cpp src/system/threading/ThreadPool.cpp src/system/threading/ThreadPool.h src/networking/protocols/DNS.cpp src/networking/protocols/DNS.h servers/DNS/DNS_Server.cpp servers/DNS/DNS_Server.h src/util/uuid/uuid.h src/networking/util/serialization.h servers/util/ConfigParser.h servers/util/StartupConfManager.h servers/util/StartupConfManager.cpp servers/util/ConfigParser.cpp)

find_package(Boost 1.78.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Add the winsock library if the host is on windows
if(WIN32)
    target_link_libraries(dev_dynamo wsock32 ws2_32)
endif()