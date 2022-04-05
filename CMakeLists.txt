cmake_minimum_required(VERSION 3.21)
project(dev_dynamo)

set(CMAKE_CXX_STANDARD 14)

add_executable(dev_dynamo main.cpp src/data-structures/common/Node.h src/data-structures/linked-lists/LinkedList.h src/data-structures/trees/BinarySearchTree.h src/data-structures/queue/Queue.h src/networking/server/servers.h src/networking/server/WebServer.cpp src/networking/protocols/HTTP.cpp src/networking/protocols/HTTP.h src/networking/server/WebServer.h src/data-structures/data-structures.h src/networking/networking.h src/system/system.h src/networking/sockets/common/Socket.cpp src/networking/sockets/common/Socket.h src/networking/sockets/sockets.h src/networking/sockets/SocketBinder.cpp src/networking/sockets/SocketBinder.h src/networking/sockets/SocketConnector.cpp src/networking/sockets/SocketConnector.h src/networking/server/BasicServer.cpp src/networking/server/BasicServer.h src/networking/sockets/SocketListener.cpp src/networking/sockets/SocketListener.h src/data-structures/dictionary/common/Entry.h src/data-structures/trees/default-comparisons/string-compare.h src/data-structures/trees/default-comparisons/int-compare.h src/data-structures/trees/default-comparisons/string-keys-compare.h src/data-structures/dictionary/Dictionary.h src/system/time/Time.cpp src/system/time/Time.h src/system/threading/ThreadPool.cpp src/system/threading/ThreadPool.cpp src/system/threading/ThreadPool.h)

if(WIN32)
    target_link_libraries(dev_dynamo wsock32 ws2_32)
endif()