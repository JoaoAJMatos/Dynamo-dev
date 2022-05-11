//
// Created by joaoa on 05/04/2022.
//

#include "DNS_Server.h"

/* HELPER FUNCTIONS */
// Saves the server config into a file at the default location
void servers::DNS_Server::save_config() const
{
    // Store the configurations and set the ENV variable
    std::string full_path = DEFAULT_CONFIG_PATH FILE_NAME;
    
    // Write the configs to the file
    config::set_config(full_path, "domain", std::to_string(this->domain), true);
    config::set_config(full_path, "service", std::to_string(this->service), true);
    config::set_config(full_path, "protocol", std::to_string(this->protocol), true);
    config::set_config(full_path, "port", std::to_string(this->port), true);
    config::set_config(full_path, "backlog", std::to_string(this->backlog), true);
    config::set_config(full_path, "threads", std::to_string(this->number_of_threads), true);
}

/* CONSTRUCTOR/DESTRUCTOR */
servers::DNS_Server::DNS_Server(int domain, int service, int protocol, int port, unsigned long iface, int bklg, int thread_count)
: net::BasicServer(domain, service, protocol, port, iface, bklg)
{
    logger("Fetching config file");
    Time::sleep(500);

    std::string full_path = DEFAULT_CONFIG_PATH FILE_NAME;

    std::map<std::string, std::string> server_config;
    server_config = config::getConfigFromFile(full_path);

    // Check if there are any saved configs
    if(!(server_config.empty()))
    {
        logger("Config file found!");
        std::cout << "[" << Time::getCurrentDateTime() << "][+] Using configuration found in " << full_path << std::endl;
        logger("Fetching...");

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
    // new config variable for the config file path
    else
    {
        logger("No config file found, booting from scratch");

        /* CREATE THE CONFIG DIRECTORIES */
        logger("Creating config directories...");

        #ifdef _WIN32
            if(!(std::filesystem::create_directory(DEFAULT_CONFIG_PATH))) exit(-1);
        #else
            if(int res = std::system("mkdir -p /etc/dynamo/DDNS/") < 0) exit(-1);
        #endif

        logger("Config directories created!");

        // Get path to the config file
        std::string config_file_path;

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

            config_file_path = DEFAULT_CONFIG_PATH;
        }
        // Prompt the user for the server configs
        else if (config_flag == 'N' || config_flag == 'n')
        {
            std::cout << std::endl << "[+] Enter path to store the config file (without file name): ";
            std::cin >> config_file_path;
            std::cout << std::endl;

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
        }

        logger("Saving config");
        save_config();
    }

    logger("Starting name server...");

    std::cout << "   - Initializing thread pool..." << std::endl;

    // Create the thread pool
    tp = new sys::ThreadPool(number_of_threads);

    std::cout << "   - Thread pool started" << std::endl;
    std::cout << "   - " << tp->get_number_of_threads() << " threads awaiting work" << std::endl << std::endl;

    logger("Name server successfully started");

    /* OPENING DATABASE */

    logger("Opening database");

    int res = sqlite3_open(KNOWN_HOSTS_LIST_PATH, &db);

    if(res)
    {
        std::cout << "Unable to open the database: " << sqlite3_errmsg(db) << std::endl;
        exit(1);
    }

    // Attempt to create the table if it as not been created
    // TABLE STRUCTURE:
    // ____________________________
    // | UUID | IP | PORT | LINKS |
    sql = "CREATE TABLE IF NOT EXISTS hosts(uuid CHAR(36) PRIMARY KEY NOT NULL, ip CHAR(15) NOT NULL, port INTEGER UNSIGNED NOT NULL, links INTEGER UNSIGNED)";

    res = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);

    if (res != SQLITE_OK)
    {
        std::cout << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
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

    nodeIP = inet_ntoa(address.sin_addr);

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
    if (DNS_query)
    {
        std::string messageBuffer;

        // If the server receives a SYNC-ME type query:
        // Create a message buffer containing the hosts the node should be assigned to. Send the request, and update the known hosts list if needed
        if (DNS_query->get_type() == SYNC_ME)
        {
            /* NODE FETCHING */
            // Look for the first 10 nodes with the least amount of connected peers
            if(!known_hosts.empty())
            {
                // Sort array items
                //std::sort(known_hosts.begin(), known_hosts.end(), compare);

                // Get the top 10 least connected peers, or all the peers if length is less than 10
                for (int i = 0; i < std::min((int)known_hosts.size(), 10); ++i)
                {
                    // Build the message buffer
                    //
                    // The message buffer should look something like:      192.168.1.109:1500;192.168.1.110:79;... (where ';' separates the nodes)
                    //messageBuffer += known_hosts[i].second.nodeIP + ":" + std::to_string(known_hosts[i].second.port) + ";";
                }
            }
            else
            {
                // If there are no previous nodes, it means the incoming node is the root node
                messageBuffer = "root";
            }

            /* UPDATING KNOWN HOSTS LIST */
            // Check if the incoming node has already been registered in the known hosts list, and if not, add it
            //std::string full_path = KNOWN_HOSTS_LIST_PATH DIR_SEP KNOWN_HOSTS_FILE_NAME;

            // The expression to look for is in the format: "node IP address=port"
            std::string expression;
            expression.append(nodeIP);
            expression.append("=");
            expression.append(DNS_query->get_body());

            // Add the new node to the known hosts list
            //if(!(exists_in_file(full_path.data(), expression.data())))
            {
                // Build the string to store in the file

                //config::set_config(KNOWN_HOSTS_LIST_PATH, KNOWN_HOSTS_FILE_NAME, DNS_query->get_source(), DNS_query->get_body());
            }
        }

        // Send the response
        send(new_socket, messageBuffer.data(), strlen(messageBuffer.data()), 0);
        close(new_socket);
        return;
    }

    char* hello = "Hello from server";
    send(new_socket, hello, strlen(hello), 0);
    close(new_socket);
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
std::map<std::string, std::string> servers::DNS_Server::get_known_hosts()
{
    return known_hosts;
}
