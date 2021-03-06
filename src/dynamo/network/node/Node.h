//
// Created by joaoa on 21/04/2022.
//

#ifndef DEV_DYNAMO_NODE_H
#define DEV_DYNAMO_NODE_H

#define NODE_CONFIG_FILE_NAME "node.conf"
#define DEFAULT_UNIX_NODE_CONFIG_PATH "/etc/dynamo/Node/"
#define DEFAULT_WINDOWS_NODE_CONFIG_PATH "C:\\dynamo\\Node"
#define BLOCKCHAIN_DB_PATH "/etc/dynamo/Node/blockchain.db"

#ifdef _WIN32
#define DEFAULT_CONFIG_PATH DEFAULT_WINDOWS_NODE_CONFIG_PATH
#define FULL_STARTUP_PATH FULL_STARTUP_PATH_WINDOWS
#else
#define DEFAULT_CONFIG_NODE_PATH DEFAULT_UNIX_NODE_CONFIG_PATH
#define FULL_STARTUP_PATH FULL_STARTUP_PATH_UNIX
#endif // _WIN32

#define MAX_BUFFER_LENGTH 512
#define DDNS1 "127.0.0.1" // Name server 1's IP
#define DDNS1_PORT 4542
#define MAX_ATTEMPTS 3

#include "./common/NodeServer.h"
#include "./common/NodeClient.h"
#include "../protocol/DTP.h"
#include "../../../networking/protocols/DDNS/DNS.h"
#include "../../../util/file-handling/config-handling/ConfigParser.h"
#include "../../../util/file-handling/config-handling/StartupConfManager.h"
#include "../../blockchain/Blockchain.h"
#include "../../miner/Miner.h"
#include "../../wallet/Wallet.h"
#include "../../wallet/transaction/TransactionPool.h"
#include "../../wallet/Wallet.h"
#include "../../../util/std-out/logger.h"
#include "../../../util/uuid/uuid.h"
#include "../../../system/time/Time.h"

#include <map>
#include <iostream>
#include <sqlite3.h>
#include <filesystem>
#include <thread>

typedef std::vector<std::pair<std::string, int>> hostMap;

class Node
{
private:
    /* MEMBER VARIABLES */

    // Variables regarding both socket descriptors
    // since both the server and the client use the same address family, service, and protocol
    int domain;
    int service;
    int protocol;
    int server_port;
    unsigned long server_interface;
    int backlog;
    int number_of_threads;
    char uuid[38];

    bool isChainLinked;
    bool isPoolLinked;

    // Array of all the known hosts
    // This map will store the IP address and port of all the server nodes
    // This map will be filled in after the node syncs its state with the name server
    hostMap known_hosts;

    // Map of all the default DDNS servers
    hostMap known_DDNS;

    // Node Server & Client instances
    // These instances will be launched in separate threads
    //
    NodeClient* client;     // Launched on the main thread
    NodeServer* server;     // Launched on the second thread

    Wallet* wallet;                   // Wallet instance
    Blockchain* blockchain;           // Blockchain instance
    TransactionPool* transactionPool; // Transaction pool instance
    Miner* miner;                     // Miner instance
    std::thread* miner_thread;         // Thread for the miner instance

    bool has_created_miner = false;
    bool mine;                    // Indicate if the miner should mine or not
    int mineLog;                  // Indicate if the miner should log or not

    int broadcast_flag;           // Flag to indicate if the node is broadcasting
    std::string broadcast_buffer; // The message to broadcast
    int sendBlock_flag = 0;       // Flag to indicate if the node is sending a block

    std::string notification_buffer;

    std::string root_address;

    /* PRIVATE FUNCTIONS */
    // This function will save the server and client configurations
    void save_config() const;

    void showBalance();
    void showKnownLinks();
    void showAddress();
    void showBlockAtIndex();
    void transact();
    void minerMenu();
    void showServerConfig();
    void showRootAddress();

public:
    /* CONSTRUCTOR/DESTRUCTOR */
    // The constructor takes in the configuration for the server and client instances
    Node();
    ~Node();

    /* PUBLIC FUNCTIONS */
    // This function will fetch the known hosts list from the default name server
    int discover_peers();

    // This function broadcasts a message to all the known hosts
    int broadcast(const std::string& message);

    int syncChains();
    int syncPool();
    int createWallet();
    int receive_file(int sockfd, char* filename);
    void broadCastHandler();
    int notify_root(); // Notifies the name servers that my address is the root node address
    int get_root_address(); // Gets the root node address from the name servers

    /**
     * @brief This function is responsible for taking the input from the user and processing it
     * 
     */
    void getInput();

    /**
     * @brief This function is responsible for the node's main loop
     * 
     */
    void start();
};


#endif //DEV_DYNAMO_NODE_H
