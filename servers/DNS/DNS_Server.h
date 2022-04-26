//
// Created by joaoa on 05/04/2022.
//

#ifndef DEV_DYNAMO_DNS_SERVER_H
#define DEV_DYNAMO_DNS_SERVER_H

#define UNIX_STARTUP_CONFIG_PATH "/etc/dynamo/DDNS/startup.dycfg"
#define WINDOWS_STARTUP_CONFIG_PATH "C:\\dynamo\\ddns\\startup\\startup.dycfg"
#define DEFAULT_UNIX_CONFIG_PATH "/etc/dynamo/DDNS/server/config"
#define DEFAULT_WINDOWS_CONFIG_PATH "C:\\dynamo\\ddns\\server\\config"


#ifdef _WIN32
#define STARTUP_CONFIG_PATH WINDOWS_STARTUP_CONFIG_PATH
#define DEFAULT_CONFIG_PATH DEFAULT_WINDOWS_CONFIG_PATH
#else
#define STARTUP_CONFIG_PATH UNIX_STARTUP_CONFIG_PATH
#define DEFAULT_CONFIG_PATH DEFAULT_UNIX_CONFIG_PATH
#endif // _WIN32

#define BUFFER_SIZE 30000
#define CONF_PATH "DDNS_CONF_PATH"

#include <map>
#include <unistd.h>
#include <sstream>
#include <cstdlib>

#include "../../src/system/threading/ThreadPool.h"
#include "../../src/system/time/Time.h"
#include "../../src/networking/sockets/common/Socket.h"
#include "../../src/networking/objects/servers.h"
#include "../../src/networking/protocols/DDNS/DNS.h"
#include "../../src/util/uuid/uuid.h"
#include "../../src/util/std-out/logger.h"
#include "../../src/util/file-handling/config-handling/ConfigParser.h"
#include "../../src/util/file-handling/config-handling/StartupConfManager.h"
#include "DNS_Server.h"

namespace servers
{
    class DNS_Server: public net::BasicServer
    {
    private:
        /* MEMBER VARIABLES */
        // Dictionary of all the known hosts
        //    - Key: UUID of the node
        //    - Object: Socket object
        std::unordered_map<std::string, std::string> known_hosts;

        // Dictionary for timestamping connections
        //    - Key: UUID of the node
        //    - Object: Timestamp of the last connection made
        std::map<std::string, std::time_t> connection_table;

        // Variables regarding the server
        int domain;
        int service;
        int protocol;
        int port;
        unsigned long iface;
        int backlog;
        char ip_str[INET_ADDRSTRLEN];

        // An instance of a DNS protocol class
        net::DNS* DNS_query;

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

        // This function saves the server configs in the config file
        void save_config(const std::string& file_path) const;

    public:
        /* CONSTRUCTOR */
        // The constructor for the class takes in the expected parameters to create a basic server
        // defined in BasicServer.h, as well as the number of threads the server will have in the thread
        // pool. If the number of threads isn't specified, the default value is used.
        DNS_Server(int domain, int service, int protocol, int port, unsigned long iface, int bklg, int thread_count = DEFAULT_THREAD_COUNT);
        ~DNS_Server();

        /* GETTERS */
        std::unordered_map<std::string, std::string> get_known_hosts();
        std::map<std::string, std::time_t> get_connection_table();

        /* PUBLIC FUNCTIONS */
        // This function launches the server, pushing the thread jobs to the thread pool
        void launch() override;
    };
}


#endif //DEV_DYNAMO_DNS_SERVER_H
