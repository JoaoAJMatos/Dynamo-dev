//
// Created by joaoa on 05/04/2022.
//

#ifndef DEV_DYNAMO_DNS_SERVER_H
#define DEV_DYNAMO_DNS_SERVER_H

#define WINDOWS_DIR_SEP "\\"
#define UNIX_DIR_SEP "/"
#define FILE_NAME "dynamo.dycfg"
#define DEFAULT_UNIX_CONFIG_PATH "/etc/dynamo/DDNS/"
#define DEFAULT_WINDOWS_CONFIG_PATH "C:\\dynamo\\ddns"
#define WINDOWS_KNOWN_HOSTS_LIST_PATH "C:\\dynamo\\ddns\\known_hosts.db"
#define UNIX_KNOWN_HOSTS_LIST_PATH "/etc/dynamo/DDNS/known_hosts.db"

#ifdef _WIN32
#define DEFAULT_CONFIG_PATH DEFAULT_WINDOWS_CONFIG_PATH
#define KNOWN_HOSTS_LIST_PATH WINDOWS_KNOWN_HOSTS_LIST_PATH
#define DIR_SEP WINDOWS_DIR_SEP
#else
#define DEFAULT_CONFIG_PATH DEFAULT_UNIX_CONFIG_PATH
#define KNOWN_HOSTS_LIST_PATH UNIX_KNOWN_HOSTS_LIST_PATH
#define DIR_SEP WINDOWS_DIR_SEP
#endif // _WIN32

#define BUFFER_SIZE 30000
#define CONF_PATH "DDNS_CONF_PATH"

#include <map>
#include <unistd.h>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <sqlite3.h>
#include <filesystem>

#ifndef _WIN32
#include <arpa/inet.h>
#endif

#include "../../src/system/threading/ThreadPool.h"
#include "../../src/system/time/Time.h"
#include "../../src/networking/sockets/common/Socket.h"
#include "../../src/networking/objects/servers.h"
#include "../../src/networking/protocols/DDNS/DNS.h"
#include "../../src/util/uuid/uuid.h"
#include "../../src/util/std-out/logger.h"
#include "../../src/util/file-handling/config-handling/ConfigParser.h"
#include "../../src/util/file-handling/config-handling/StartupConfManager.h"
#include "../../src/util/file-handling/exprFinder.h"

namespace servers
{
    class DNS_Server: public net::BasicServer
    {
    private:
        /* MEMBER VARIABLES */
        // Array of all the known hosts
        // Each element conatins a string identifying the Node's IP and port, like: "192.168.1.109:1500"
        std::vector<std::string> known_hosts;

        // Variables regarding the server
        int domain;
        int service;
        int protocol;
        int port;
        unsigned long iface;
        int backlog;
        char ip_str[INET_ADDRSTRLEN];
        char* nodeIP;

        // SQLite3 instance
        sqlite3* db;

        // An instance of a DNS protocol class
        net::DNS* DNS_query;
        char* zErrMsg = 0;
        char* sql;

        unsigned long new_socket;

        // Message buffer
        // This buffer will hold the request
        char buffer[BUFFER_SIZE];

        // Multi threading and thread pool instance
        int number_of_threads;
        sys::ThreadPool* tp;

        /* VIRTUAL FUNCTION OVERRIDES */
        // TODO: Define the virtual functions
        void accepter() override;
        // The handler is used in a multi threaded environment to handle incoming connections
        void handler() override;
        void responder() override;

        /* HELPER FUNCTIONS */
        // This function saves the server configs in the config file
        void save_config() const;

    public:
        /* CONSTRUCTOR */
        // The constructor for the class takes in the expected parameters to create a basic server
        // defined in BasicServer.h, as well as the number of threads the server will have in the thread
        // pool. If the number of threads isn't specified, the default value is used.
        DNS_Server(int domain, int service, int protocol, int port, unsigned long iface, int bklg, int thread_count = DEFAULT_THREAD_COUNT);
        ~DNS_Server();

        /* GETTERS */
        std::map<std::string, std::string> get_known_hosts();

        /* PUBLIC FUNCTIONS */
        // This function launches the server, pushing the thread jobs to the thread pool
        void launch() override;
    };
}


#endif //DEV_DYNAMO_DNS_SERVER_H
