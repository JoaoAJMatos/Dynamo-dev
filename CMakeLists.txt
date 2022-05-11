# Set cmake version, project name and version
cmake_minimum_required(VERSION 3.21)
project(dev_dynamo LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

# Set cmake generator
set(CMAKE_GENERATOR "Ninja")

add_executable(dev_dynamo main.cpp src/networking/objects/servers.h src/networking/protocols/HTTP/HTTP.cpp src/networking/protocols/HTTP/HTTP.h src/networking/networking.h src/system/system.h src/networking/sockets/common/Socket.cpp src/networking/sockets/common/Socket.h src/networking/sockets/sockets.h src/networking/sockets/SocketBinder.cpp src/networking/sockets/SocketBinder.h src/networking/sockets/SocketConnector.cpp src/networking/sockets/SocketConnector.h src/networking/objects/BasicServer.cpp src/networking/objects/BasicServer.h src/networking/sockets/SocketListener.cpp src/networking/sockets/SocketListener.h src/system/time/Time.cpp src/system/time/Time.h src/system/threading/ThreadPool.cpp src/system/threading/ThreadPool.cpp src/system/threading/ThreadPool.h src/networking/protocols/DDNS/DNS.cpp src/networking/protocols/DDNS/DNS.h servers/DNS/DNS_Server.cpp servers/DNS/DNS_Server.h src/util/uuid/uuid.h src/util/file-handling/config-handling/ConfigParser.h src/util/file-handling/config-handling/StartupConfManager.h src/util/file-handling/config-handling/StartupConfManager.cpp src/util/file-handling/config-handling/ConfigParser.cpp src/networking/objects/BasicClient.cpp src/networking/objects/BasicClient.h src/networking/network/node/common/NodeClient.cpp src/networking/network/node/common/NodeClient.h src/networking/network/node/Node.cpp src/networking/network/node/Node.h src/networking/network/node/common/NodeServer.cpp src/networking/network/node/common/NodeServer.h src/util/std-out/logger.h src/util/std-out/logger.cpp src/dynamo/block/Block.cpp src/dynamo/block/Block.h src/crypto/SHA256.h src/crypto/SHA256.cpp src/dynamo/wallet/transaction/Transaction.cpp src/dynamo/wallet/transaction/Transaction.h src/dynamo/wallet/Wallet.cpp src/dynamo/wallet/Wallet.h src/crypto/secp256k1/secp256k1.h src/util/file-handling/exprFinder.cpp src/util/file-handling/exprFinder.h src/util/uuid/uuid.cpp)

find_package(OpenSSL REQUIRED)

target_link_libraries(dev_dynamo OpenSSL::Crypto sqlite3)

# Add the winsock library if the host is on windows
if(WIN32)
    target_link_libraries(dev_dynamo wsock32 ws2_32)
endif()
