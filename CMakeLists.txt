# Set cmake version, project name and version
cmake_minimum_required(VERSION 3.13)
project(dev_dynamo LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -lz -lpthread")

add_executable(
        dev_dynamo
        main.cpp

        src/networking/objects/servers.h
        src/networking/protocols/HTTP/HTTP.cpp
        src/networking/protocols/HTTP/HTTP.h
        src/networking/networking.h
        src/networking/sockets/common/Socket.cpp
        src/networking/sockets/common/Socket.h
        src/networking/sockets/sockets.h
        src/networking/sockets/SocketBinder.cpp
        src/networking/sockets/SocketBinder.h
        src/networking/sockets/SocketConnector.cpp
        src/networking/sockets/SocketConnector.h
        src/networking/objects/BasicServer.cpp
        src/networking/objects/BasicServer.h
        src/networking/sockets/SocketListener.cpp
        src/networking/sockets/SocketListener.h
        src/networking/protocols/DDNS/DNS.cpp
        src/networking/protocols/DDNS/DNS.h
        src/networking/objects/BasicClient.cpp
        src/networking/objects/BasicClient.h

        src/dynamo/block/Block.h
        src/dynamo/block/Block.cpp
        src/dynamo/blockchain/Blockchain.h
        src/dynamo/blockchain/Blockchain.cpp
        src/dynamo/miner/Miner.cpp
        src/dynamo/miner/Miner.h
        src/dynamo/wallet/Wallet.h
        src/dynamo/wallet/Wallet.cpp
        src/dynamo/wallet/transaction/Transaction.h
        src/dynamo/wallet/transaction/Transaction.cpp
        src/dynamo/wallet/transaction/TransactionPool.h
        src/dynamo/wallet/transaction/TransactionPool.cpp
        src/dynamo/network/node/common/NodeClient.cpp
        src/dynamo/network/node/common/NodeClient.h
        src/dynamo/network/node/common/NodeServer.cpp
        src/dynamo/network/node/common/NodeServer.h
        src/dynamo/network/node/Node.cpp
        src/dynamo/network/node/Node.h
        src/dynamo/consensus/consensus.hpp

        src/system/system.h
        src/system/time/Time.cpp
        src/system/time/Time.h
        src/system/threading/ThreadPool.cpp
        src/system/threading/ThreadPool.h

        src/util/uuid/uuid.h
        src/util/file-handling/config-handling/ConfigParser.h
        src/util/file-handling/config-handling/StartupConfManager.h
        src/util/file-handling/config-handling/StartupConfManager.cpp
        src/util/file-handling/config-handling/ConfigParser.cpp
        src/util/std-out/logger.h
        src/util/std-out/logger.cpp
        src/util/file-handling/exprFinder.cpp
        src/util/file-handling/exprFinder.h
        src/util/uuid/uuid.cpp

        src/crypto/SHA-2/SHA256.cpp
        src/crypto/SHA-2/SHA256.h
        src/crypto/EC/ecc.h
        src/crypto/EC/ecc.cpp
        src/crypto/EC/ECDSA.cpp
        src/crypto/EC/ECDSA.h

        servers/DNS/DNS_Server.h
        servers/DNS/DNS_Server.cpp
)

find_package(OpenSSL REQUIRED)

target_link_libraries(
    dev_dynamo 
    OpenSSL::Crypto 
    sqlite3
)

# Add the winsock library if the host is on windows
if(WIN32)
    target_link_libraries(dev_dynamo wsock32 ws2_32)
endif()
