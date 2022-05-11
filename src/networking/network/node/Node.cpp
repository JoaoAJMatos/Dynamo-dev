//
// Created by joaoa on 21/04/2022.
//

#include "Node.h"

/* CONSTRUCTOR */
Node::Node()
{
    logger("Node startup sequence initiated");
    Time::sleep(500);

    // Fetch the config file
    std::map<std::string, std::string> startup_config;
    startup_config = config::getConfigFromFile(FULL_STARTUP_PATH);

    // Flags for startup verification
    // True if the config file was found, and false otherwise
    bool server_config_found = startup_config.find(CONFIG_PATH) != startup_config.end();

    // Check if the startup config was found, if not, boot from scratch
    if(server_config_found)
    {
        // Inform the user that the config files were found
        logger("Config files found...");
        std::cout << "[" << Time::getCurrentDateTime() << "][+] Booting server using configuration found in " << startup_config[CONFIG_PATH] << std::endl;
        logger("Fetching configs...");

        // Fetch the path for the config file found inside the startup config file
        std::map<std::string, std::string> node_config;

        node_config = config::getConfigFromFile(startup_config[CONFIG_PATH]);

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

        // Ask the user if he wants to use the default configurations or not
        char use_default;

        // Path to store the config file
        std::string node_config_path;

        std::cout << "[+] Use default Node settings (recommended)? (y/n): ";
        std::cin >> use_default;

        if (use_default == 'y' || use_default == 'Y')
        {
            // Use the default configuration
            logger("Using default configuration for the server");
            node_config_path = DEFAULT_CONFIG_PATH;

            // Set server variables
            this->server_port = 1500;
            this->backlog = 100;
            this->number_of_threads = 0;
        }
        else if (use_default == 'n' || use_default == 'N')
        {
            // Prompt the user for the settings and set the server configs
            logger("Using custom server configs...");

            std::cout << "[+] Enter path to config file (configs will be saved here): ";
            std::cin >> node_config_path;

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
        save_config(node_config_path);
    }
}

/* DESTRUCTOR */
Node::~Node()
{
    delete client;
    delete server;
}

/* PRIVATE FUNCTIONS */
void Node::save_config(const std::string &config_file_path) const
{
    // Store the configurations and set the ENV variable
    std::string full_path = config_file_path + "\\node.conf";

    // Create and open the file
    std::ofstream config_file(full_path);

    // Write to the file
    config_file << "domain=" << this->domain << std::endl;
    config_file << "service=" << this->service << std::endl;
    config_file << "protocol=" << this->protocol << std::endl;
    config_file << "port=" << this->server_port << std::endl;
    config_file << "backlog=" << this->backlog << std::endl;
    config_file << "threads=" << this->number_of_threads << std::endl;
    config_file << "uuid=" << this->uuid;

    // Close the file
    config_file.close();

    // TODO: Fix this!
    //config::set_config(NODE_STARTUP_CONFIG_PATH, STARTUP_CONFIG_FILE_NAME, CONFIG_PATH, full_path);
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
        result = client->request("192.168.1.109", 4542, query.get_string());
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
