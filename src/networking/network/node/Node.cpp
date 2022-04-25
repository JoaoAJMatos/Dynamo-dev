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
    std::map<std::string, std::string> server_startup_config;
    server_startup_config = config::getConfigFromFile(SERVER_STARTUP_CONFIG_PATH);

    std::map<std::string, std::string> client_startup_config;
    server_startup_config = config::getConfigFromFile(CLIENT_STARTUP_CONFIG_PATH);

    // Flags for startup verification
    // True if the config file was found, and false otherwise
    bool server_config_found = server_startup_config.find(SERVER_CONF_PATH) != server_startup_config.end();
    bool client_config_found = client_startup_config.find(CLIENT_CONF_PATH) != client_startup_config.end();

    // Check if the startup config was found, if not, boot from scratch
    if(server_config_found and client_config_found)
    {
        // Inform the user that the config files were found
        logger("Config files found...");
        std::cout << "[" << Time::getCurrentDateTime() << "][+] Booting server using configuration found in " << SERVER_STARTUP_CONFIG_PATH << std::endl;
        std::cout << "[" << Time::getCurrentDateTime() << "][+] Booting client using configuration found in " << CLIENT_STARTUP_CONFIG_PATH << std::endl;
        logger("Fetching configs...");

        // Fetch the path for the config file found inside the startup config file
        std::map<std::string, std::string> server_config;
        std::map<std::string, std::string> client_config;

        server_config = config::getConfigFromFile(server_startup_config[SERVER_CONF_PATH]);
        client_config = config::getConfigFromFile(client_startup_config[CLIENT_CONF_PATH]);

        logger("Done!");
        logger("Setting Node variables...");

        // The domain, service, and protocol can be fetched from either the server or the client, since they are the same
        this->domain = atoi(server_config["domain"].c_str());
        this->service = atoi(server_config["service"].c_str());
        this->protocol = atoi(server_config["protocol"].c_str());

        // Fetch the server port
        this->server_port = atoi(server_config["port"].c_str());

        // Set the interface for the server
        this->server_interface = INADDR_ANY;

        // Set the backlog and number of threads for the server
        this->backlog = atoi(server_config["backlog"].c_str());
        this->number_of_threads = atoi(server_config["threads"].c_str());

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

        std::cout << "[+] Use default Node settings (recommended)? (y/n): ";
        std::cin >> use_default;

        if (use_default == 'y' || use_default == 'Y')
        {
            // Use the default configuration
            logger("Using default configuration for the server");

            // Set server variables
            this->domain = AF_INET;
            this->service = SOCK_STREAM;
            this->protocol = IPPROTO_TCP;
            this->server_port = 1500;
            this->server_interface = INADDR_ANY;
            this->backlog = 100;
            this->number_of_threads = 0;
        }
        else if (use_default == 'n' || use_default == 'N')
        {
            // Prompt the user for the settings and set the server configs

        }


        logger("Server configured successfully");
        logger("Initiating client and server instances");

        // Create instances of NodeClient and NodeServer
        this->client = new NodeClient(this->domain, this->service, this->protocol);
        this->server = new NodeServer(this->domain, this->service, this->protocol, this->server_port, this->server_interface, this->backlog, this->number_of_threads);

        logger("Saving config");

        // Save the configs in the config file
    }
}

/* PUBLIC FUNCTIONS */
// This function will send a DDNS query to one of the pre-defined
void Node::discover_peers()
{

}

void Node::save_config(const std::string &config_file_path) const
{

}
