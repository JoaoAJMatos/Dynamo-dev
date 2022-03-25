cmake_minimum_required(VERSION 3.21)
project(dev_dynamo)

set(CMAKE_CXX_STANDARD 14)

add_executable(dev_dynamo main.cpp src/data-structures/linked-lists/Node.h src/data-structures/linked-lists/LinkedList.h src/data-structures/queue/Queue.h src/data-structures/data-structures.h src/networking/sockets/common/Socket.cpp src/networking/sockets/common/Socket.h src/networking/sockets/sockets.h src/networking/sockets/SocketBinder.cpp src/networking/sockets/SocketBinder.h src/networking/sockets/SocketConnector.cpp src/networking/sockets/SocketConnector.h src/networking/networking.h src/networking/sockets/SocketListener.cpp src/networking/sockets/SocketListener.h src/networking/server/WebServer.cpp src/networking/server/WebServer.h src/networking/server/servers.h)

if(WIN32)
    target_link_libraries(dev_dynamo wsock32 ws2_32)
endif()