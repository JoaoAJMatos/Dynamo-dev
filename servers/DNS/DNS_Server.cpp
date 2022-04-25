//
// Created by joaoa on 05/04/2022.
//

#include "DNS_Server.h"

void servers::DNS_Server::save_config(const std::string& config_file_path) const
{
    // Store the configurations and set the ENV variable
    std::string full_path = config_file_path + "\\ddns.conf";

    config::set_config(full_path, "domain", (char*)(this->domain));
    config::set_config(full_path, "service", (char*)(this->service));
    config::set_config(full_path, "protocol", (char*)(this->protocol));
    config::set_config(full_path, "port", (char*)(this->port));
    config::set_config(full_path, "interface", this->ip_str);
    config::set_config(full_path, "backlog", (char*)(this->backlog));
    config::set_config(full_path, "threads", (char*)(this->number_of_threads));


    // Add config path to the ENV variables
    config::set_config(STARTUP_CONFIG_PATH ,CONF_PATH, full_path.data());
}

/* CONSTRUCTOR/DESTRUCTOR */
servers::DNS_Server::DNS_Server(int domain, int service, int protocol, int port, unsigned long iface, int bklg, int thread_count)
: net::BasicServer(domain, service, protocol, port, iface, bklg)
{
    logger("Fetching config file");
    Time::sleep(500);

    // Check if a path to a config file is specified in the startup config file
    std::map<std::string, std::string> startup_configs;
    startup_configs = config::getConfigFromFile(STARTUP_CONFIG_PATH);

    // If the path for the config file is found, fetch the config file and set the server settings
    if(startup_configs.find(CONF_PATH) != startup_configs.end())
    {
        logger("Config file found!");
        std::cout << "[" << Time::getCurrentDateTime() << "][+] Using configuration found in " << STARTUP_CONFIG_PATH << std::endl;
        logger("Fetching...");

        std::map<std::string, std::string> server_config;
        server_config = config::getConfigFromFile(startup_configs[CONF_PATH]);

        logger("Done!");
        logger("Setting server variables...");

        this->domain = atoi(server_config["domain"].c_str());
        this->service = atoi(server_config["service"].c_str());
        this->protocol = atoi(server_config["protocol"].c_str());
        this->port = atoi(server_config["port"].c_str());

        sockaddr_in sa{};
        inet_pton(AF_INET, server_config["interface"].c_str(), &(sa.sin_addr));
        this->iface = sa.sin_addr.s_addr;

        this->backlog = atoi(server_config["backlog"].c_str());
        this->number_of_threads = atoi(server_config["threads"].c_str());

        logger("Ready to launch!");
        std::cout << std::endl;
    }
    // If the config file is not found, prompt the user for the path to the file, set the server variables and create a
    // new ENV variable for the config file path
    else
    {
        logger("No config file found, booting from scratch");

        // Get path to the config file
        std::string config_file_path;
        std::cout << std::endl << "[+] Enter path to store the config file (without file name): ";
        std::cin >> config_file_path;
        std::cout << std::endl;

        // Check if the user wants to use the default settings or not
        char config_flag;
        std::cout << "[+] Would you like to use the default configuration for the server? (Y/N): ";
        std::cin >> config_flag;
        std::cout << std::endl;

        // Use the default configs
        if(config_flag == 'Y' || config_flag == 'y')
        {
            this->domain = AF_INET;
            this->service = SOCK_STREAM;
            this->protocol = IPPROTO_TCP;
            this->port = 4542;

            this->iface = INADDR_ANY;

            this->backlog = 100;
            this->number_of_threads = 0;

            save_config(config_file_path);
        }
        // Prompt the user for the server configs
        else if (config_flag == 'N' || config_flag == 'n')
        {
            int temp;
            std::cout << "[+] Address family [IPv4/IPv6] (Choose 1 or 2): ";
            std::cin >> temp;

            if(temp == 1)
            {
                this->domain = AF_INET;
                std::cout << "[SERVER INFO] Server address family set to IPv4" << std::endl;
            }
            else if (temp == 2)
            {
                this->domain = AF_INET6;
                std::cout << "[SERVER INFO] Server address family set to IPv6" << std::endl;
            }

            std::cout << "[+] Service [TCP/UDP] (Choose 1 or 2): ";
            std::cin >> temp;

            if(temp == 1)
            {
                this->service = SOCK_STREAM;
                std::cout << "[SERVER INFO] Server service set to TCP" << std::endl;
            }
            else if(temp == 2)
            {
                this->service = SOCK_DGRAM;
                std::cout << "[SERVER INFO] Server service set to UDP" << std::endl;
            }

            this->protocol = 0;

            std::cout << "[+] Port: ";
            std::cin >> this->port;

            this->iface = INADDR_ANY;

            std::cout << "[+] Server backlog: ";
            std::cin >> this->backlog;

            std::cout << "[+] Thread count (enter 0 if you are not sure): ";
            std::cin >> this->number_of_threads;

            save_config(config_file_path);
        }
    }

    logger("Starting name server...");

    std::cout << "   - Initializing thread pool..." << std::endl;

    // Create the thread pool
    tp = new sys::ThreadPool(number_of_threads);

    std::cout << "   - Thread pool started" << std::endl;
    std::cout << "   - " << tp->get_number_of_threads() << " threads awaiting work" << std::endl << std::endl;

    logger("Name server successfully started");
}

servers::DNS_Server::~DNS_Server()
{
    delete tp;
}

/* MEMBER FUNCTIONS */
void servers::DNS_Server::accepter()
{
    // Socket address struct
    struct sockaddr_in address = get_socket()->get_address();

    // Length of the address struct to be passed into the accept() function
    int addrlen = sizeof(address);

    // Store the new socket
    new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
    net::Socket::test_connection(new_socket);

    logger("Client connected!");

    // Read the incoming message and store it in the buffer
    recv(new_socket, buffer, BUFFER_SIZE, 0);
}

// The handler function is responsible for handling the incoming request and parsing it to create
// a new DNS request class instance.
void servers::DNS_Server::handler()
{
    // Try to create a DNS query instance from the incoming buffer
    try
    {
        DNS_query = new net::DNS(buffer);
    }
    catch (std::exception& e)
    {
        logger("Unable to create DNS query instance from incoming buffer");
    }

    std::cout << buffer << std::endl;
}

// The responder will analyze the DNS query and respond accordingly
void servers::DNS_Server::responder()
{
    // If the server receives a SYNC-ME type query:
    // Create a message buffer containing all the known hosts and send it to the client
    if (DNS_query)
    {
        if (DNS_query->get_type() == SYNC_ME)
        {
            // Create message buffer
            char* hello = "Sync me";
            send(new_socket, hello, strlen(hello), 0);
        }
    }

    char* hello = "Hello from server";
    send(new_socket, hello, strlen(hello), 0);
    closesocket(new_socket);
}

/* PUBLIC FUNCTIONS */
// This function launches the server
void servers::DNS_Server::launch()
{
    logger("Launch sequence initiated...");
    std::cout << "[" << Time::getCurrentDateTime() << "][+] Server listening on port " << this->port << std::endl;
    logger("Logging incoming connections...");

    tp->do_work([&]() {
        while(true)
        {
            accepter();
            handler();
            responder();
        }
    });
}

/* GETTERS */
// Returns a dictionary containing all the known hosts
std::unordered_map<std::string, std::string> servers::DNS_Server::get_known_hosts()
{
    return known_hosts;
}

// Returns a dictionary containing all the active connections
std::map<std::string, std::time_t> servers::DNS_Server::get_connection_table()
{
    return connection_table;
}

