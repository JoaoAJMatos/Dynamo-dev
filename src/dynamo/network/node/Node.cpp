//
// Created by joaoa on 21/04/2022.
//

#include "Node.h"

/* HELPER FUNCTIONS */
void toFileNode(std::string content)
{
    std::ofstream tempFile;
    tempFile.open("temp.txt");
    tempFile << content;
    tempFile.close();
}

std::string fromFileNode()
{
    std::string line;
    std::ifstream myfile("temp.txt");
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            myfile.close();
            return line;
        }
    }
}

/* CONSTRUCTOR */
Node::Node()
{
    logger("Node startup sequence initiated");
    Time::sleep(500);

    isChainLinked = false;

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
            this->server_port = 4568;
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

    // Set the list of known DDNS
    known_DDNS.emplace_back("192.168.1.120", 4542);
    known_DDNS.emplace_back("127.0.0.1", 4542);

    // Set broadcast flag to 0 and clear the broadcast buffer
    broadcast_flag = 0;
    broadcast_buffer.clear();

    this->transactionPool = new TransactionPool();
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

    // Attempt to fetch the list of known hosts from a server from the default list
    for (auto const& [ip, port]: known_DDNS)
    {
        std::cout << "[+] Fetching from [" << ip << ":" << port << "]" << std::endl;

        // Make 3 attempts at making the request
        for (i = 1; i <= MAX_ATTEMPTS; i++)
        {
            // Send the request to the server and wait for a response
            result = client->request(ip.c_str(), port, query.get_string());
            if (result == 0) break; // Exit the loop if the connection was successful
            if (result < 0 && i == MAX_ATTEMPTS) return -1; // Return if the connection wasn't successful and the maximum amount of tries was exceeded
            logger("Connection unsuccessful, trying again...");
        }
        
        if (result == 0) break;
    }

    return 0;
}

// This function will send a given message to all the known hosts
int Node::broadcast(const std::string& message)
{
    // Loop through the list of known hosts and send the message
    for (auto& node : known_hosts)
    {
        std::cout << "[+] Sending to [" << node.first << ":" << node.second << "]" << std::endl;
        this->client->request(node.first.data(), node.second, message);
    }

    return 0;
}

int Node::createWallet()
{
    char hasWallet;
    bool isWalletCreated = false;

    logger("Creating wallet...");

    while (!(isWalletCreated))
    {
        std::cout << std::endl << std::endl << "[+] Do you already have a wallet? (y/n): ";
        std::cin >> hasWallet;
        std::cout << std::endl;

        if (hasWallet == 'Y' || hasWallet == 'y') // Login to the wallet
        {
            std::string pubKey;
            std::string privKey;

            std::cout << "[+] Enter your wallet's address: ";
            std::cin >> pubKey;

            std::cout << "[+] Enter your wallet's private key: ";
            std::cin >> privKey;

            this->wallet = new Wallet(pubKey.data(), privKey.data());

            std::cout << "[+] Login successful" << std::endl << std::endl;
        
            isWalletCreated = true;
        }
        else if (hasWallet == 'N' || hasWallet == 'n') // Create a new wallet
        {
            std::cout << "[+] Generating a new ECDSA key pair..." << std::endl;
            this->wallet = new Wallet();

            std::cout << "[+] Wallet created successfully!" << std::endl;
            std::cout << "[+] Your wallet's address is: " << this->wallet->getAddress() << std::endl;
            std::cout << "[+] Your wallet's private key is: " << this->wallet->getPrivate() << std::endl << std::endl;

            std::cout << "[NOTE]  The above parameters must be saved in a safe place, as they are required to login to the wallet. \n" 
                                 "\tDynamo is not responsible for recovering any lost login parameters. If you loose your private key,\n"
                                 "\tWE CANNOT RECOVER YOUR WALLET." << std::endl << std::endl;

            #ifdef _WIN32
                system("pause");
            #else
                std::cout << "Press any key to continue..." << std::endl;
                system("read");
            #endif

            isWalletCreated = true;
        }
    }

    return 0;
}

void Node::showBalance()
{
    int balance = Wallet::calculateBalance(this->blockchain, this->wallet->getAddress());
    std::cout << "[+] Your balance: " << balance << " D¥N" << std::endl;
}

void Node::showKnownLinks()
{
    int count = known_hosts.size();

    if (count > 0)
    {
        std::cout << "[+] Linked to " << count << " nodes:" << std::endl;

        for (auto& node : known_hosts)
        {
            std::cout << "-> " << node.first << ":" << node.second << std::endl;
        }

        return;
    }

    std::cout << "[+] You are not connected to any nodes" << std::endl;
}

void showHelp()
{
    std::cout << std::endl;
    std::cout << "Dynamo (shell) | Node Interaction Interface" << std::endl;
    std::cout << "Usage: [command] [options]" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  clear\t\t\tClear the console" << std::endl;
    std::cout << "  help\t\t\tShow this help message" << std::endl;
    std::cout << "  balance\t\tShow your wallet's balance" << std::endl;
    std::cout << "  links\t\t\tList all known nodes" << std::endl;
    std::cout << "  address\t\tShows your wallet's address" << std::endl;
    std::cout << "  show-chain\t\tPrints the blockchain" << std::endl;
    std::cout << "  show-block\t\tPrint a block at a given index" << std::endl;
    std::cout << "  transaction-pool\tShows the transaction pool" << std::endl;
    std::cout << "  transact\t\tConduct a transaction" << std::endl;
    std::cout << "  exit\t\t\tExits the program" << std::endl;
    std::cout << std::endl;
}

void Node::showAddress()
{
    std::cout << "[+] Your wallet's address is: " << this->wallet->getAddress() << std::endl;
}

void Node::showBlockAtIndex()
{
    std::cout << "[+] Block height: ";
    int index;
    std::cin >> index;
    this->blockchain->printBlock(index);
}

void Node::transact()
{
    int balance = Wallet::calculateBalance(this->blockchain, this->wallet->getAddress());

    std::cout << std::endl << "[+] Conduct Transaction" << std::endl;
    std::cout << "[+] Your balance: " << balance << " D¥N" << std::endl;

    std::cout << std::endl << "[ADDRESS]: ";
    std::string address;
    std::cin >> address;

    std::cout << "[AMOUNT]: ";
    int amount;
    std::cin >> amount;

    std::cout << std::endl << "[+] Are you sure you want to send " << amount << " D¥N to " << address << "? (y/n): ";
    char confirm;
    std::cin >> confirm;

    if (confirm == 'Y' || confirm == 'y')
    {   
        Transaction* transaction = this->wallet->createTransaction(address, amount, this->blockchain);

        if (transaction == nullptr)
        {
            std::cout << "[WARNING] Insufficient funds: " << balance << " D¥N" << std::endl << std::endl;
            return;
        }

        std::cout << "[+] Sending " << amount << " D¥N to '" << address << "'" << std::endl;
        std::cout << "[ID]: " << transaction->getID() << std::endl << std::endl;

        std::string transactionString = Transaction::toString(transaction);
        this->transactionPool->setTransaction(transaction);

        std::cout << "Transaction string payload sent: " << transactionString << std::endl;

        DTP::Packet packet(TRANSACTION_PACKET, this->uuid, std::string("all"), this->server_port, 0, transactionString);

        broadcast(packet.buffer());
    }
    else
    {
        std::cout << "[+] Transaction cancelled" << std::endl;
        return;
    }
}

void Node::getInput()
{
    std::string input;
    std::cout << std::endl << Time::getCurrentDateTime() << " | Dynamo v.1" <<std::endl;
    std::cout << "Type 'help' for a list of all the available commands" << std::endl;

    while (true)
    {
        std::cout << "dynamo@" << this->uuid << ":~$ ";
        std::cin >> input;

        if (input == "clear") system("clear");
        else if (input == "balance") showBalance();
        else if (input == "links") showKnownLinks();
        else if (input == "address") showAddress();
        else if (input == "show-chain") this->blockchain->printChain();
        else if (input == "show-block") showBlockAtIndex();
        else if (input == "transaction-pool") this->transactionPool->show();
        else if (input == "transact") transact();
        else if (input == "exit") exit(0);
        else if (input == "help") showHelp();
        else std::cout << "Unknown command '" << input << "'" << std::endl;
    }
}

int Node::send_file(FILE* fp, int sockfd)
{
    char data[PACKET_SIZE] = {0};

    while(fgets(data, PACKET_SIZE, fp) != NULL)
    {
        if(send(sockfd, data, sizeof(data), 0) < 0)
        {
            std::cout << "[ERROR] (At NodeServer::send_file(2)): Failed to send file" << std::endl;
            return -1;
        }
        bzero(data, PACKET_SIZE);
    }

    return 0;
}

int Node::receive_file(int sockfd)
{
    int n;
    FILE* fp;
    char* filename = "temp.txt";
    char buffer[PACKET_SIZE];
    int bytesReceived = 0;

    fp = fopen(filename, "ab");
    if (fp == nullptr)
    {
        std::cout << "[ERROR] (At NodeServer::receive_file(1)): Failed to open file for writing" << std::endl;
        return -1;
    }

    long double sz = 1;

    while((bytesReceived = recv(sockfd, buffer, PACKET_SIZE, 0)) > 0)
    {
        sz++;
        gotoxy(0, 4);
        printf("Received: %llf Mb", (sz/PACKET_SIZE));
        fflush(stdout);
        fwrite(buffer, 1, bytesReceived, fp);
    }

    if (bytesReceived < 0)
    {
        std::cout << "[ERROR] (At NodeServer::receive_file(2)): Failed to read from socket" << std::endl;
        return -1;
    }

    return 0;
}

int Node::syncChains()
{
    // Keep asking the known nodes for an updated blockchain until one is received
    for (auto& node : known_hosts)
    {
        DTP::Packet packet(1, this->uuid, node.first, this->server_port, node.second, std::string(""));

        int res = this->client->request(node.first.c_str(), node.second, packet.buffer());

        if (res == 0)
        {
            try
            {
                DTP::Packet response(client->get_response_buffer());

                if (response.getIndicator() == DTP_INDICATOR && response.headers().type == BLOCKCHAIN_DATA_PACKET)
                {
                    // Send the FTP ready packet to start the file transfer. The payload indicates what file should be sent
                    DTP::Packet pkt(FTP_READY, this->uuid, node.first, this->server_port, node.second, std::string("1"));
                    res = this->client->request(node.first.c_str(), node.second, pkt.buffer());

                    if (res == 0)
                    {
                        res = receive_file(client->get_sock());

                        if (res == 0)
                        {
                            std::string blockchain_string = fromFileNode();
                            this->blockchain = new Blockchain(blockchain_string);
                            if (this->blockchain->chain.empty()) return -1;
                            this->isChainLinked = true;
                            logger("Chain synced successfully");
                            return 0;
                        }
                    }
                }
            }
            catch(const std::exception& e)
            {
                logger("Error while parsing blockchain");
            }
        }
        else 
        {
            logger("Error while syncing chain");
        }
    }

    return -1;
}

void Node::start()
{
    logger("Initiating client and server instances");

    // Create instances of NodeClient and NodeServer
    this->client = new NodeClient(this->domain, this->service, this->protocol);
    this->server = new NodeServer(this->domain, this->service, this->protocol, this->server_port, this->server_interface, this->backlog, this->number_of_threads);

    // Start the server in a new thread
    logger("Server node launch sequence initiated");
    std::thread server_thread(&NodeServer::launch, this->server);
    server_thread.detach();

    // Create the node wallet
    createWallet();

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

    /* RESPONSE PARSING */
    // Parse the response buffer after the Syncronization request to get the list of known hosts
    std::string tempResponse = this->client->get_response_buffer();

    // If the response is "root" then we can start the blockchain and insert the Genesis Block
    if (tempResponse == "root")
    {
        logger("You are the network's root node!");
        logger("Initializing the blockchain...");

        this->blockchain = new Blockchain(1, this->wallet->getAddress());
        isChainLinked = true;
    }
    else // Else: Loop through the string and parse the request
    {
        std::string delimiter = ";"; // Delimiter for the body contents of the response
        std::string ip_port_del = ":"; 
        size_t pos = 0;

        while ((pos = tempResponse.find(delimiter)) != std::string::npos)
        {
            std::string ip = tempResponse.substr(0, tempResponse.find(ip_port_del));
            int port = atoi(tempResponse.substr(tempResponse.find(ip_port_del) + ip_port_del.length(), pos).data());
            tempResponse.erase(0, pos + delimiter.length());

            known_hosts.emplace_back(ip, port);
        }

        std::cout << "[+] Linking with " << known_hosts.size() << " nodes..." << std::endl;

        logger("Syncing with the latest blockchain...");

        while(!isChainLinked)
        {
            int result = syncChains();

            if (result < 0)
            {
                logger("Unable to sync with the network");
                logger("Retrying...");
                Time::sleep(1000);
            }
        }
    }

    if (isChainLinked)
    {
        logger("Blockchain synced successfully!");
        
        this->server->set_working_blockchain(this->blockchain);
        this->server->set_working_transaction_pool(this->transactionPool);
        this->server->set_node_uuid(this->uuid);
        this->server->set_known_hosts(&this->known_hosts);

        // Start the node's input loop
        getInput();
    }
}
