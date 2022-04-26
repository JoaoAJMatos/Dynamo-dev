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
    startup_config = config::getConfigFromFile(NODE_STARTUP_CONFIG_PATH);

    // Flags for startup verification
    // True if the config file was found, and false otherwise
    bool server_config_found = startup_config.find(NODE_STARTUP_CONFIG_PATH) != startup_config.end();

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

        logger("Server configured successfully");
        logger("Initiating client and server instances");

        // Create instances of NodeClient and NodeServer
        this->client = new NodeClient(this->domain, this->service, this->protocol);
        this->server = new NodeServer(this->domain, this->service, this->protocol, this->server_port, this->server_interface, this->backlog, this->number_of_threads);

        logger("Saving config");

        // Save the configs in the config file
        save_config(node_config_path);
    }
}

/* PRIVATE FUNCTIONS */
void Node::save_config(const std::string &config_file_path) const
{
    std::string full_path = config_file_path + "//node.conf";

    config::set_config(full_path, "domain", (char*)(this->domain));
    config::set_config(full_path, "service", (char*)(this->service));
    config::set_config(full_path, "protocol", (char*)(this->protocol));
    config::set_config(full_path, "port", (char*)(this->server_port));
    config::set_config(full_path, "backlog", (char*)(this->backlog));
    config::set_config(full_path, "threads", (char*)(this->number_of_threads));
}


/* PUBLIC FUNCTIONS */
// This function will send a DDNS query to one of the pre-defined
void Node::discover_peers()
{

}