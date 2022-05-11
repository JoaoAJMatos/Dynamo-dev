//
// Created by joaoa on 21/04/2022.
//

#include "Node.h"

/* CONSTRUCTOR */
Node::Node()
{
    logger("Node startup sequence initiated");
    Time::sleep(500);

    std::string full_path = DEFAULT_CONFIG_PATH NODE_CONFIG_FILE_NAME;

    // Fetch the configs from the config file
    std::map<std::string, std::string> node_config;
    node_config = config::getConfigFromFile(full_path);

    // Check if the startup config was found, if not, boot from scratch
    if(!(node_config.empty()))
    {
        // Inform the user that the config files were found
        logger("Config files found...");
        std::cout << "[" << Time::getCurrentDateTime() << "][+] Booting server using configuration found in " << full_path << std::endl;
        logger("Fetching configs...");

        logger("Done!");
        logger("Setting Node variables...");

        // The domain, service, and protocol can be fetched from either the server or the client, since they are the same
        this->domain = atoi(node_config["domain"].c_str());
        this->service = atoi(node_config["service"].c_str());
        this->protocol = atoi(node_config["protocol"].c_str());

        // Fetch the server port
        this->server_port = atoi(node_config["port"].c_str());

        // Set the interface for the server
        this->server_interface = INADDR_ANY;

        // Set the backlog and number of threads for the server
        this->backlog = atoi(node_config["backlog"].c_str());
        this->number_of_threads = atoi(node_config["threads"].c_str());

        strcpy(this->uuid, node_config["uuid"].c_str());

        logger("Ready to launch!");

        // Create instances of NodeClient and NodeServer
        this->client = new NodeClient(this->domain, this->service, this->protocol);
        this->server = new NodeServer(this->domain, this->service, this->protocol, this->server_port, this->server_interface, this->backlog, this->number_of_threads);
    }
    else
    {
        logger("No config files were found. Booting from scratch...");

        logger("Creating config files");

        /* Create config directories */
        #ifdef _WIN32
            if(!(std::filesystem::create_directory(DEFAULT_CONFIG_PATH))) exit(-1);
        #else
            if(int res = std::system("mkdir -p /etc/dynamo/Node/") < 0) exit(-1);
        #endif


        // Ask the user if he wants to use the default configurations or not
        char use_default;


        std::cout << "[+] Use default Node settings (recommended)? (y/n): ";
        std::cin >> use_default;

        if (use_default == 'y' || use_default == 'Y')
        {
            // Use the default configuration
            logger("Using default configuration for the server");

            // Set server variables
            this->server_port = 1500;
            this->backlog = 100;
            this->number_of_threads = 0;
        }
        else if (use_default == 'n' || use_default == 'N')
        {
            // Prompt the user for the settings and set the server configs
            logger("Using custom server configs...");

            std::cout << "[+] Server port: ";
            std::cin >> this->server_port;
            std::cout << "[+] Backlog: ";
            std::cin >> this->backlog;
            std::cout << "[+] Threads: ";
            std::cin >> this->number_of_threads;
        }

        // These variables are common to both the client and the server
        // They are the default configs and cannot be changed
        this->domain = AF_INET;
        this->service = SOCK_STREAM;
        this->protocol = IPPROTO_TCP;
        this->server_interface = INADDR_ANY;

        // Create the Node UUID
        Dynamo_UUID::uuidv4(this->uuid);

        logger("Server configured successfully");

        logger("Saving config");

        // Save the configs in the config file
        save_config();
    }
}

/* DESTRUCTOR */
Node::~Node()
{
    delete client;
    delete server;
}

/* PRIVATE FUNCTIONS */
void Node::save_config() const
{
    std::string full_path = DEFAULT_CONFIG_PATH NODE_CONFIG_FILE_NAME;

    std::cout << full_path;

    // Write config to file
    config::set_config(full_path, "domain", std::to_string(this->domain), true);
    config::set_config(full_path, "service", std::to_string(this->service), true);
    config::set_config(full_path, "protocol", std::to_string(this->protocol), true);
    config::set_config(full_path, "port", std::to_string(this->server_port), true);
    config::set_config(full_path, "port", std::to_string(this->server_port), true);
    config::set_config(full_path, "threads", std::to_string(this->number_of_threads), true);
    config::set_config(full_path, "uuid", this->uuid, true);
}


/* PUBLIC FUNCTIONS */
// This function will send a DDNS query to one of the pre-defined name servers
int Node::discover_peers()
{
    logger("Syncing known node's list...");

    int result; // Variable to store the result of the request function
    int i; // For loop iterator

    // Build the DNS query
    net::DNS query(SYNC_ME, this->uuid, std::to_string(this->server_port));

    // Make 3 attempts at making the request
    for (i = 1; i <= MAX_ATTEMPTS; i++)
    {
        // Send the request to the server and wait for a response
        result = client->request("127.0.0.1", 4542, query.get_string());
        if (result == 0) break; // Exit the loop if the connection was successful
        if (result < 0 && i == MAX_ATTEMPTS) return -1; // Return if the connection wasn't successful and the maximum amount of tries was exceeded
        logger("Connection unsuccessful, trying again...");
    }

    return 0;
}

void Node::start()
{
    logger("Initiating client and server instances");

    // Create instances of NodeClient and NodeServer
    this->client = new NodeClient(this->domain, this->service, this->protocol);
    this->server = new NodeServer(this->domain, this->service, this->protocol, this->server_port, this->server_interface, this->backlog, this->number_of_threads);

    // Sync the known hosts list
    int result = discover_peers();

    if (result < 0)
    {
        std::cout << std::endl;
        logger("The node was unable to fetch a list of known hosts...");
        logger("Unable to connect to the network");
        logger("Exiting");
        exit(-1);
    }
}
